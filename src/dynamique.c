
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * dynamique classique NVE, NVT, DPD et DPDE                                 * 
 *                                                                           * 
 * algorithme de verlet vitesse                                              * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "dynamique.h"

int dynamique( int idmethode ) {

  int  erreur = EXIT_SUCCESS ;

  numerostep = 1 ;

  // impressions initiales
  entete( idmethode ) ;
  moyenne( idmethode ) ;

  // boucle globale sur le pas de temps
  while ( numerostep <= nstep ) {

    // tirage a l'avance
    // tirage() ;

    /* un pas positions et 1/2 pas vitesse */
    position( idmethode ) ;

    /* conditions periodiques aux limites */
    periodic_cond() ;
 
    /* mise a jour des voisin si necessaire */
    if ( majvoisin > 0 ) voisin() ;

    /* calcul des forces et de l'energie */
    force_ene() ;

    /* DPD sans splitting : modification des forces */
    force_DPD( idmethode, idDPDnosplit ) ;

    /* 1/2 pas vitesse */
    vitesse( idmethode ) ;

    /* DPD splitting : modification des vitesses */
    force_DPD( idmethode, idDPDsplit ) ;
    
    /* modification des vitesses et des energies internes : DPDE */
    force_DPDE( idmethode ) ;
    
    // impressions sur le .dat et calculs divers
    if ( nave != 0 && numerostep % nave == 0 ) moyenne( idmethode );

    // impression conf
    if ( ncrd != 0 && numerostep % ncrd == 0 ) print_conf() ;

    // impression vitesse
    if ( nvel != 0 && numerostep % nvel == 0 ) print_vel() ;
   
    numerostep++ ; 
  }
  
  // impressions finales
  print_radial_dist() ;
  print_fin( idmethode ) ;
  print_restart() ;

  // liberation memoire
  free_memoire() ;

  return erreur ;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * entete dynamique                                                          * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int entete( int idmethode ) {

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

