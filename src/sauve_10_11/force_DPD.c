/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* force dissipative et force aleatoire pour la DPD                          */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

extern	double	gaussienne( void ) ;

int force_DPD( double dist2, int iat, int jat ) {

	int 	erreur = 0 ;
	double	dist ;
	double	sigma, racdt, amplitude ;
	double	chi, chi2 ;
	double	dvx, dvy, dvz ;
	double	fdpdx, fdpdy, fdpdz ;
	double	wx, wy, wz ;

	sigma = sqrt( 2. * xsi * boltzman * temp0 ) ;
	racdt = sqrt( dt ) ;
	amplitude = sigma / racdt ; 		// homogene a une force

	// chi : fonction de cutt off simple = lineaire
	dist = sqrt(dist2) ;
	chi = 1. - dist / rcut ;
	chi2 = chi * chi ;

	// tirage
	wx = gaussienne() ;
	wy = gaussienne() ;
	wz = gaussienne() ;

	// friction et force aleatoire
	dvx = ( vx[iat] - vx[jat] ) ;
	dvy = ( vy[iat] - vy[jat] ) ;
	dvz = ( vz[iat] - vz[jat] ) ;

	fdpdx = - xsi * chi2 * dvx + amplitude * chi * wx ;
	fdpdy = - xsi * chi2 * dvy + amplitude * chi * wy ;
	fdpdz = - xsi * chi2 * dvz + amplitude * chi * wz ;

	frcx[iat] += fdpdx ;
	frcy[iat] += fdpdy ;
	frcz[iat] += fdpdz ;

	frcx[jat] -= fdpdx ;
	frcy[jat] -= fdpdy ;
	frcz[jat] -= fdpdz ;

	return	erreur ;
}

