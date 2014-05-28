/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* mise à jour des positions et 1/2 pas vitesse dans le cas NVE              */
/*                                                                           */
/* algorithme de verlet vitesse                                              */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

int positionNVE ( void ) {

	int	erreur = 0 ;
	int	iat ;
	double	mass_inv ;
	double	x2, y2, z2 ;
	double	deplacement ;
	
	mass_inv = 0.5 * dt / masse ;
	majvoisin = 0 ;

	for ( iat = 0 ; iat < nat ; iat++ ) {
	
		// vitesse a t + 1/2 dt
		vx[iat] += mass_inv * frcx[iat] ;		
		vy[iat] += mass_inv * frcy[iat] ;		
		vz[iat] += mass_inv * frcz[iat] ;		

		// position a t + dt utilisant les vitesses a t+1/2
		x2 = x[iat] + vx[iat] * dt ;
		y2 = y[iat] + vy[iat] * dt ;
		z2 = z[iat] + vz[iat] * dt ;

		// test rafraichissement de la table de voisins
		dx[iat] += fabs( x2 - x[iat] ) ;
		dy[iat] += fabs( y2 - y[iat] ) ;
		dz[iat] += fabs( z2 - z[iat] ) ;

		deplacement = dx[iat] * dx[iat] + dy[iat] * dy[iat] + dz[iat] * dz[iat] ;

		if ( deplacement > dmax2 ) majvoisin++ ;

		// maj des positions
		x[iat] = x2 ;
		y[iat] = y2 ;
		z[iat] = z2 ;
	}

	return erreur;
}
