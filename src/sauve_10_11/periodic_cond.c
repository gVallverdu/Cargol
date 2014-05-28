/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* application des conditions periodiaues aux limites                        */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

int periodic_cond ( void ) {

	int	erreur = 0 ;
	int	iat ;
	
	for ( iat = 0 ; iat < nat ; iat++ ) {
	
		x[iat] = x[iat] - boxx * (double) ( (int) ( x[iat] / boxx ) ) ;
		y[iat] = y[iat] - boxy * (double) ( (int) ( y[iat] / boxy ) ) ;
		z[iat] = z[iat] - boxz * (double) ( (int) ( z[iat] / boxz ) ) ;

	}
	
	return erreur;
}

