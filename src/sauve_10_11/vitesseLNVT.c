/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* second 1/2 pas vitesse incluant la friction et le deplacement aleatoire   */
/* implementation de langevin stamp3                                         */
/* algorithme de verlet vitesse                                              */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

extern	double	gaussienne ( void ) ;

int vitesseLNVT ( void ) {

	int	erreur = 0 ;
	int	iat ;
	
	double	mass_inv, sigma, gamma ;
	
	mass_inv = 0.5 * dt / masse ;

	gamma = 0.5 * xsi * dt ;
	sigma = sqrt( ( 1. + gamma ) * 2. * xsi * boltzman * temp0 / dt ) ;

	for ( iat = 0 ; iat < nat ; iat++ ) {
				
		/* rajout de la force aleatoire */
		frcx[iat] += sigma * sqrt(masse) * gaussienne() ;	
		frcy[iat] += sigma * sqrt(masse) * gaussienne() ;
		frcz[iat] += sigma * sqrt(masse) * gaussienne() ;

		/* dernier demi pas vitesse */
		vx[iat] = ( vx[iat] + mass_inv * frcx[iat] ) / ( 1. + 0.5 * dt * xsi ) ;
		vy[iat] = ( vy[iat] + mass_inv * frcy[iat] ) / ( 1. + 0.5 * dt * xsi ) ;
		vz[iat] = ( vz[iat] + mass_inv * frcz[iat] ) / ( 1. + 0.5 * dt * xsi ) ;

	}
	
	return erreur;
}

