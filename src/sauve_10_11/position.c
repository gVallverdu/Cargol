/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* mise a jour des positions et 1/2 pas vitesse                              */
/*                                                                           */
/* algorithme de verlet vitesse                                              */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"

extern	int	positionNVE( void ) ;
extern	int	positionLNVT( void ) ;

int position ( int idmethode ) {

	int	erreur = 0 ;

	if ( idmethode == idNVE ) {
		positionNVE() ;

	} else if ( idmethode == idlangevin ) {
		positionNVE() ;

	} else if ( idmethode == idLNVT ) {
		positionLNVT() ;

	} else if ( idmethode == idDPD ) {
		positionNVE() ;

	} else if ( idmethode == idDPDE ) {
		positionNVE() ;

	}

	return erreur;
}
