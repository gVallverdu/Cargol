
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * initialisation de l'energie interne et de la temperature interne : DPDE   * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"

int init_DPDE ( int Natom, Data *d, Particules *Particule[] ) {

  Particules *p = NULL ;
  
  int     erreur = EXIT_SUCCESS ;
  int     iat, step ;
  double  alpha, coef1, coef2, beta ;
  double  temp, temp_int ;

  beta = 1. / (d->boltzman * d->temp_interne0 ) ;			// en J
  alpha = 0.02 ; 							// sans unite, tire de stamp2
  coef1 = alpha / (d->capacite_calo * beta * beta * d->boltzman ) ;	// en K
  coef2 = sqrt( 2. * alpha ) / ( d->capacite_calo * beta ) ; 		// en K

  temp_int = 0. ;
  for ( iat = 0 ; iat < Natom ; iat++ ) {
    p = &((*Particule)[iat]) ;
  
    temp = d->temp_interne0;
    for ( step = 0 ; step < NSTEPTEMP ; step++ ) {
      temp += coef1 * ( 1. / temp - 1. / d->temp_interne0 ) + coef2 * gasdev( &(d->idum) );
    }

    if ( temp <= 1.) {
      printf("temp < 1");
      temp = 1.1 ;
    }
    p->Tint = temp ;
    temp_int += 1. / temp ;
  }
  temp_int = temp_int / (double) Natom ;
  temp_int = 1. / temp_int ;
  

  /* ouverture du fichier des temperatures et energies internes */
  d->ftemp_int = fopen( d->atemp_int, "w" ) ;
  if ( d->ftemp_int == NULL ) stop("ouverture du fichier de sortie des temperatures internes");

  /* impressions de la distribution initiale */
  fprintf( d->ftemp_int, "# 1 : indice de l'atome\n" );
  fprintf( d->ftemp_int, "# 2 : temperature interne (K), \t moyenne : %e K\n", temp_int );
  fprintf( d->ftemp_int, "# 3 : energie interne (kJ.mol-1)\n" );
  fprintf( d->ftemp_int, "& temperature et energie interne initiale \n" ) ;
  for ( iat = 0 ; iat < Natom ; iat++ ) {
    p = &((*Particule)[iat]) ;
  
    p->Eint = d->capacite_calo * p->Tint ;
    
    fprintf( d->ftemp_int, "%5d "   ,   iat ) ;
    fprintf( d->ftemp_int, "%16.7e ",   p->Tint ) ;
    fprintf( d->ftemp_int, "%16.7e \n", p->Eint * d->avogadro / 1000. ) ;
  }

  return erreur ;
}

