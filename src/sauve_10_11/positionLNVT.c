/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* mise a jour des positions et 1/2 pas vitesse                              */
/* implementation comme stamp3                                               */
/* algorithme de verlet vitesse                                              */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

int positionLNVT ( void ) {

	int	erreur = 0 ;
	int	iat ;
	double	mass_inv, mass_inv2, dtxsi ;
	double	x2, y2, z2 ;
	double	deplacement ;
	
	mass_inv  = 0.5 * dt / masse ;
	mass_inv2 = 0.5 * dt*dt / masse ;
	dtxsi     = 0.5 * xsi * dt ;

	majvoisin = 0 ;

	for ( iat = 0 ; iat < nat ; iat++ ) {
	
		// position a t + dt (pas NVE)
		x2 = x[iat] + vx[iat] * dt + mass_inv2 * frcx[iat] ;
		y2 = y[iat] + vy[iat] * dt + mass_inv2 * frcy[iat] ;
		z2 = z[iat] + vz[iat] * dt + mass_inv2 * frcz[iat] ;

		// force de friction
		x2 += - dt * dtxsi * vx[iat] ;
		y2 += - dt * dtxsi * vy[iat] ;
		z2 += - dt * dtxsi * vz[iat] ;

		// vitesse a t + 1/2 dt
		vx[iat] += mass_inv * frcx[iat] - dtxsi * vx[iat] ;
		vy[iat] += mass_inv * frcy[iat] - dtxsi * vy[iat] ;
		vz[iat] += mass_inv * frcz[iat] - dtxsi * vz[iat] ;

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

