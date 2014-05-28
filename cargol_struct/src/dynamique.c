
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * dynamique classique NVE, NVT, DPD et DPDE                                 * 
 *                                                                           * 
 * algorithme de verlet vitesse                                              * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "dynamique.h"

int dynamique( int idmethode, Data *Donnees, Etat *EtatThermo, Particules *Particule[] ) {

  int  erreur = EXIT_SUCCESS ;
  int  Natom = EtatThermo->nat ;

  Donnees->numerostep = 1 ;

  // impressions initiales
  entete( idmethode, Donnees ) ;
  moyenne( idmethode, Donnees, EtatThermo, Particule ) ;

  // boucle globale sur le pas de temps
  while ( Donnees->numerostep != Donnees->nstep ) {

    /* calcul des nouvelles positions et 1/2 pas vitesse */
    position( idmethode, Natom, Donnees, Particule ) ;

    /* conditions periodiques aux limites */
    periodic_cond( Natom, Donnees, Particule ) ;

    /* mise a jour des voisin si necessaire */
    if ( Donnees->majvoisin > 0 ) voisin( Natom, Donnees, Particule ) ;

    /* calcul des forces et de l'energie */
    force_ene( idmethode, Donnees, EtatThermo, Particule ) ;

    /* DPD sans splitting : modification des forces */
    force_DPD( idmethode, idDPDnosplit, Natom, Donnees, Particule ) ;

    /* dernier 1/2 pas vitesse */
    vitesse( idmethode, Natom, Donnees, Particule ) ;
    
    /* DPD splitting : modification des vitesses uniquement */
    force_DPD( idmethode, idDPDsplit, Natom, Donnees, Particule ) ;
    
    /* modification des vitesses et des energies internes : DPDE */
    force_DPDE( idmethode, Natom, Donnees, Particule ) ;
            
    /* impressions sur le .dat et calculs divers */
    if ( Donnees->nave != 0  &&  Donnees->numerostep % Donnees->nave == 0 ) 
		moyenne( idmethode, Donnees, EtatThermo, Particule ) ;

    /* impression conf */
    if ( Donnees->ncrd != 0  &&  Donnees->numerostep % Donnees->ncrd == 0 ) 
    		print_conf( Natom, Donnees, Particule ) ;

    /* impression vitesse */
    if ( Donnees->nvel != 0  &&  Donnees->numerostep % Donnees->nvel == 0 ) 
	    	print_vel( Natom, Donnees, Particule ) ;
   
    Donnees->numerostep++ ;
  }

  // impressions finales
  print_radial_dist( Donnees, EtatThermo ) ;
  print_fin( idmethode, Donnees, EtatThermo, Particule ) ;
  print_restart( Natom, Donnees, Particule ) ;

  // liberation memoire
  free_memoire( Donnees, Particule ) ;

  return erreur ;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * entete dynamique                                                          * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int entete ( int idmethode, Data *d ) {

  FILE   *cntrl = d->cntrl ;
  int    erreur = EXIT_SUCCESS ;
  double alpha, sigma, gamma ;

  printf("\n-----------------------------------------------------------------\n" );
  printf(" * dynamique %s : \n", d->methode ) ;
  printf("-----------------------------------------------------------------\n\n" );

  fprintf( cntrl,"\n-----------------------------------------------------------------\n" );
  fprintf( cntrl," * dynamique %s : \n", d->methode ) ;
  fprintf( cntrl,"-----------------------------------------------------------------\n\n" );
  fprintf( cntrl, "resultats : %s \n", d->amdout ) ;

  if ( idmethode == idlangevin ) {
    alpha = exp( - d->xsi * d->dt / d->masse ) ;
    sigma = sqrt( d->boltzman * d->temp0 / d->masse * ( 1. - alpha*alpha) ) ;
    fprintf( cntrl,"\nfriction, xsi                     : %e kg.s-1\n", d->xsi ) ;
    fprintf( cntrl,  "parametre langevin sans dimension : %e\n"      , alpha ) ;
    fprintf( cntrl,  "amplitude du terme aleatoire      : %e m.s-1\n", sigma ) ;

  } else if ( idmethode == idLNVT ) {
    gamma = 0.5 * d->xsi * d->dt ;
    sigma = sqrt( ( 1. + gamma ) * 2. * d->xsi * d->boltzman * d->temp0 * d->masse / d->dt ) ;
    fprintf( cntrl,"\nfriction, xsi                     : %e s-1\n", d->xsi ) ;
    fprintf( cntrl,  "parametre langevin sans dimension : %e\n"  , gamma ) ;
    fprintf( cntrl,  "amplitude du terme aleatoire      : %e N\n", sigma ) ;

  } else if ( idmethode == idDPDsplit || idmethode == idDPDnosplit ) {
    alpha = exp( - d->xsi * d->dt / d->masse ) ;
    sigma = sqrt( 2. * d->xsi * d->boltzman * d->temp0 / d->dt ) ;
    fprintf( cntrl,"\nfriction, xsi                     : %e kg.s-1\n",d->xsi ) ;
    fprintf( cntrl,  "parametre langevin sans dimension : %e\n"  , alpha ) ;
    fprintf( cntrl,  "amplitude du terme aleatoire      : %e N\n", sigma ) ;
    
  } else if ( idmethode == idDPDE || idmethode == idDPDE_alamain || idmethode == idDPDE_ito ) {
    sigma = sqrt( 2. * d->xsi * d->boltzman * d->temp0 / d->dt ) ;
    fprintf( cntrl,"\nfriction, xsi                      : %e kg.s-1\n", d->xsi ) ;
    fprintf( cntrl,  "amplitude du terme aleatoire       : %e N\n"     , sigma ) ;
    fprintf( cntrl,  "coefficient calorimetrique      Cv : %e J.K-1\n" , d->capacite_calo ) ;
    fprintf( cntrl,  "temperature interne initiale       : %e K \n"    , d->temp_interne0 ) ;

  }

  return erreur ;
}

