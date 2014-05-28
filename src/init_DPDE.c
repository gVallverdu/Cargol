
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * initialisation de l'energie interne et de la temperature interne : DPDE   * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"
#include "DPDE.h"
#include "fichier.h"

int init_DPDE ( void ) {

  int     erreur = EXIT_SUCCESS ;
  int     iat, step ;
  double  alpha, coef1, coef2, beta ;
  double  temp, temp_int ;

  beta = 1. / (boltzman * temp_interne0 ) ;			// en J
  alpha = 0.02 ; 						// sans unite, tire de stamp2
  coef1 = alpha / (capacite_calo * beta * beta * boltzman ) ;	// en K
  coef2 = sqrt( 2. * alpha ) / ( capacite_calo * beta ) ; 	// en K

  temp_int = 0. ;
  for ( iat = 0 ; iat < nat ; iat++ ) {
  
    temp = temp_interne0;
    for ( step = 0 ; step < NSTEPTEMP ; step++ ) {
      temp += coef1 * ( 1. / temp - 1. / temp_interne0 ) + coef2 * gasdev( &idum );
    }

    if ( temp <= 1.) {
      printf("temp < 1");
      temp = 1.1 ;
    }
    temp_interne[iat] = temp ;
    temp_int += 1. / temp ;
  }
  temp_int = temp_int / (double) nat ;
  temp_int = 1. / temp_int ;
  

  /* ouverture du fichier des temperatures et energies internes */
  ftemp_int = fopen( atemp_int, "w" ) ;
  if ( ftemp_int == NULL ) erreur++ ;


  /* impressions de la distribution initiale */
  fprintf( ftemp_int, "# 1 : indice de l'atome\n" );
  fprintf( ftemp_int, "# 2 : temperature interne (K), \t moyenne : %e K\n", temp_int );
  fprintf( ftemp_int, "# 3 : energie interne (kJ.mol-1)\n" );
  fprintf( ftemp_int, "& temperature et energie interne initiale \n" ) ;
  for ( iat = 0 ; iat < nat ; iat++ ) {
  
    e_interne[iat] = capacite_calo * temp_interne[iat] ;
    
    fprintf( ftemp_int, "%5d "   ,   iat ) ;
    fprintf( ftemp_int, "%16.7e ",   temp_interne[iat] ) ;
    fprintf( ftemp_int, "%16.7e \n", e_interne[iat]*avogadro/1000. ) ;
  }

  return erreur ;
}

