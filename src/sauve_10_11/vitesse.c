/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* second 1/2 pas vitesse                                                    */
/*                                                                           */
/* algorithme de verlet vitesse                                              */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"

extern	int	vitesseNVE( void );
extern	int	vitesse_langevin( void );
extern	int	vitesseLNVT( void );

int vitesse ( int idmethode ) {

	int	erreur = 0 ;

	if ( idmethode == idNVE ) {
		vitesseNVE() ;

	} else if ( idmethode == idlangevin ) {
		vitesse_langevin() ;

	} else if ( idmethode == idLNVT ) {
		vitesseLNVT() ;

	} else if ( idmethode == idDPD ) {
		vitesseNVE() ;

	} else if ( idmethode == idDPDE ) {
		vitesseNVE() ;

	} 

	return erreur;
}
