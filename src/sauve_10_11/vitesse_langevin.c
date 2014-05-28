/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* second 1/2 pas vitesse incluant la friction et le deplacement aleatoire   */
/*                                                                           */
/* algorithme de verlet vitesse                                              */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

extern	double	gaussienne ( void ) ;

int vitesse_langevin ( void ) {

	int	erreur = 0 ;
	int	iat ;
	
	double	mass_inv, alpha, sigma ;
	
	mass_inv = 0.5 * dt / masse ;

	alpha = exp( - xsi * dt / masse ) ;
	sigma = sqrt( ( 1. - alpha * alpha ) * boltzman * temp0 / masse ) ;

	for ( iat = 0 ; iat < nat ; iat++ ) {
				
		/* dernier demi pas vitesse */		
		vx[iat] += mass_inv * frcx[iat] ;
		vy[iat] += mass_inv * frcy[iat] ;
		vz[iat] += mass_inv * frcz[iat] ;

		/* ajout de la friction et de la force aleatoire */
		vx[iat] = alpha * vx[iat] + sigma * gaussienne() ;
		vy[iat] = alpha * vy[iat] + sigma * gaussienne() ;
		vz[iat] = alpha * vz[iat] + sigma * gaussienne() ;
	
	}
	
	return erreur;
}
