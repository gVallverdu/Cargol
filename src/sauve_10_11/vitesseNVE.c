/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* second 1/2 pas vitesse dans le cas NVE                                    */
/*                                                                           */
/* algorithme de verlet vitesse                                              */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

int vitesseNVE ( void ) {

	int	erreur = 0 ;
	int	iat ;
	
	double	mass_inv ;
	
	mass_inv = 0.5 * dt / masse ;

	for ( iat = 0 ; iat < nat ; iat++ ) {
				
		/* Vitesse a t + dt */		
		vx[iat] += mass_inv * frcx[iat] ;
		vy[iat] += mass_inv * frcy[iat] ;
		vz[iat] += mass_inv * frcz[iat] ;

	}
	
	return erreur;
}
