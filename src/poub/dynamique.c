
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * dynamique classique NVE, NVT, DPD et DPDE                                 * 
 *                                                                           * 
 * algorithme de verlet vitesse                                              * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "dynamique.h"

int dynamique( int idmethode ) {

  int  erreur = 0 ;
  int  step ;
  clock_t NVE1, NVE2, DPD1, DPD2, vois1, vois2 ;
  static double moy1 = 0., moy2 = 0. ;

  strcpy( nomfonction, "dynamique()" ) ;

  // impressions initiales
  entete( idmethode ) ;
  moyenne( idmethode ) ;

  numerostep = 0 ;

  // boucle globale sur le pas de temps
  for ( step = 1 ; step <= nstep ; step++ ) {

    numerostep++ ;

    // tirage a l'avance
    // tirage() ;

    NVE1 = clock() ;

    // calcul des nouvelles positions et 1/2 pas vitesse
    position( idmethode ) ;

    // conditions periodiques aux limites
    periodic_cond() ;

    // mise a jour des voisin si necessaire
    vois1 = clock() ;
    if ( majvoisin > 0 ) voisin() ;
    vois2 = clock() ;
    

    // calcul des forces et de l'energie
    force_ene( idmethode ) ;

    // dernier 1/2 pas vitesse
    vitesse( idmethode ) ;

    NVE2 = clock() ;
    moy1 += ( (double) (NVE2 - NVE1) - (double) (vois2 - vois1) ) / CLOCKS_PER_SEC ;
    
    DPD1 = clock() ;
    
    // modification des vitesses : DPD
    if ( idmethode == idDPDsplit ) force_DPD() ;

    DPD2 = clock() ;
    moy2 += ( (double) (DPD2 - DPD1) ) / CLOCKS_PER_SEC ;
    
    // modification des vitesses et des energies internes : DPDE
    if ( idmethode == idDPDE || idmethode == idDPDE_alamain ) {
      force_DPDE_alamain() ;

    } else if ( idmethode == idDPDE_ito ) {
      force_DPDE_ito() ;      
    }
    
    // impressions sur le .dat et calculs divers
    if ( nave != 0 && step % nave == 0 ) moyenne( idmethode );

    // impression conf
    if ( ncrd != 0 && step % ncrd == 0 ) print_conf() ;

    // impression vitesse
    if ( nvel != 0 && step % nvel == 0 ) print_vel() ;
    
  }

  // impressions finales
  print_radial_dist() ;
  print_fin( idmethode ) ;
  print_restart() ;

  // liberation memoire
  free_memoire() ;
  
  printf("\n\ntemps NVE = %10e \ntemps DPD = %10e \n\n", moy1 / (double) nstep, moy2 / (double) nstep ) ;

  return erreur ;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * entete dynamique                                                          * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int entete ( int idmethode ) {

  int    erreur = 0 ;
  double alpha, sigma, gamma ;

  printf("\n-----------------------------------------------------------------\n" );
  printf(" * dynamique %s : \n", methode ) ;
  printf("-----------------------------------------------------------------\n\n" );

  fprintf( cntrl,"\n-----------------------------------------------------------------\n" );
  fprintf( cntrl," * dynamique %s : \n", methode ) ;
  fprintf( cntrl,"-----------------------------------------------------------------\n\n" );
  fprintf( cntrl, "resultats : %s \n", amdout ) ;

  if ( idmethode == idlangevin ) {
    alpha = exp( - xsi * dt / masse ) ;
    sigma = sqrt( boltzman * temp0 / masse * ( 1. - alpha*alpha) ) ;
    fprintf( cntrl,"\nfriction, xsi                     : %e kg.s-1\n", xsi ) ;
    fprintf( cntrl,  "parametre langevin sans dimension : %e\n"      , alpha ) ;
    fprintf( cntrl,  "amplitude du terme aleatoire      : %e m.s-1\n", sigma ) ;

  } else if ( idmethode == idLNVT ) {
    gamma = 0.5 * xsi * dt ;
    sigma = sqrt( ( 1. + gamma ) * 2. * xsi * boltzman * temp0 * masse / dt ) ;
    fprintf( cntrl,"\nfriction, xsi                     : %e s-1\n", xsi ) ;
    fprintf( cntrl,  "parametre langevin sans dimension : %e\n"  , gamma ) ;
    fprintf( cntrl,  "amplitude du terme aleatoire      : %e N\n", sigma ) ;

  } else if ( idmethode == idDPDsplit || idmethode == idDPDnosplit ) {
    alpha = exp( - xsi * dt / masse ) ;
    sigma = sqrt( 2. * xsi * boltzman * temp0 / dt ) ;
    fprintf( cntrl,"\nfriction, xsi                     : %e kg.s-1\n",xsi ) ;
    fprintf( cntrl,  "parametre langevin sans dimension : %e\n"  , alpha ) ;
    fprintf( cntrl,  "amplitude du terme aleatoire      : %e N\n", sigma ) ;
    
  } else if ( idmethode == idDPDE || idmethode == idDPDE_alamain || idmethode == idDPDE_ito ) {
    sigma = sqrt( 2. * xsi * boltzman * temp0 / dt ) ;
    fprintf( cntrl,"\nfriction, xsi                      : %e kg.s-1\n", xsi ) ;
    fprintf( cntrl,  "amplitude du terme aleatoire       : %e N\n"     , sigma ) ;
    fprintf( cntrl,  "coefficient calorimetrique      Cv : %e J.K-1\n" , capacite_calo ) ;
    fprintf( cntrl,  "temperature interne initiale       : %e K \n"    , temp_interne0 ) ;

  }

  return erreur ;
}

