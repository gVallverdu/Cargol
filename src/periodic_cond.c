
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * application des conditions periodiaues aux limites                        * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

int periodic_cond ( void ) {

  int erreur = EXIT_SUCCESS ;
  int iat ;

  for ( iat = 0 ; iat < nat ; iat++ ) {

    x[iat] = x[iat] - boxx * (double) ( floor( x[iat] / boxx ) ) ;
    y[iat] = y[iat] - boxy * (double) ( floor( y[iat] / boxy ) ) ;
    z[iat] = z[iat] - boxz * (double) ( floor( z[iat] / boxz ) ) ;

    // test
    if ( x[iat] > boxx || x[iat] < 0. ) erreur++ ;
    if ( y[iat] > boxy || y[iat] < 0. ) erreur++ ;
    if ( z[iat] > boxz || z[iat] < 0. ) erreur++ ;
    if ( erreur != 0 ) {
      printf("\n\niteration %d : ", numerostep);
      printf("atome %d en dehors de la boite\n", iat );
      printf("%10.4f ", x[iat] * 1.e10 ) ;
      printf("%10.4f ", y[iat] * 1.e10 ) ;
      printf("%10.4f \n\n", z[iat] * 1.e10 ) ;
      stop("erreur dans periodic_cond.c") ;
    }
  }
	
  return erreur;
}

