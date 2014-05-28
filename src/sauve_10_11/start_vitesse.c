/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* Tirages des vitesses initiales                                            */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

extern	double	gaussienne( void ) ;

int start_vitesse( void ) {

	int	erreur = 0 ;
	int	iat ;
	double	sigma ;
	double	ecinet[3], scaling[3] ;

	sigma = sqrt( 2. * boltzman * temp0 / masse ) ;

	massetot = 0. ;
	vgx = vgy = vgz = 0. ;

	// tirage des vitesses et calcul de la vitesse du centre de gravite
	for ( iat = 0 ; iat < nat ; iat++ ) {

		vx[iat] = sigma * gaussienne() ;
		vy[iat] = sigma * gaussienne() ;
		vz[iat] = sigma * gaussienne() ;

		massetot += masse ;

		// vitesse centre de gravite
		vgx += masse * vx[iat] ;
		vgy += masse * vy[iat] ;
		vgz += masse * vz[iat] ;
	}

	vgx = vgx / massetot ;
	vgy = vgy / massetot ;
	vgz = vgz / massetot ;

	// calcul de ecinet et suppresion de la vitesse du centre de gravite
	ecinet[0] = ecinet[1] = ecinet[2] = 0. ;
	for ( iat = 0 ; iat < nat ; iat++ ) {
		vx[iat] = vx[iat] - vgx ;
		vy[iat] = vy[iat] - vgy ;
		vz[iat] = vz[iat] - vgz ;

		// E cinetique
		ecinet[0] += 0.5 * masse * vx[iat] * vx[iat] ;
		ecinet[1] += 0.5 * masse * vy[iat] * vy[iat] ;
		ecinet[2] += 0.5 * masse * vz[iat] * vz[iat] ;
	}

	// on ajuste les vitesses a la temperature temp0
	// on a nat degres de liberte par direction
	scaling[0] = sqrt( 0.5 * (double) nat * boltzman * temp0 / ecinet[0] ) ;
	scaling[1] = sqrt( 0.5 * (double) nat * boltzman * temp0 / ecinet[1] ) ;
	scaling[2] = sqrt( 0.5 * (double) nat * boltzman * temp0 / ecinet[2] ) ;

	ecinet[0] = ecinet[1] = ecinet[2] = 0. ;
	vgx = vgy = vgz = 0. ;
	for ( iat = 0 ; iat < nat ; iat++ ) {
		vx[iat] = scaling[0] * vx[iat] ;
		vy[iat] = scaling[1] * vy[iat] ;
		vz[iat] = scaling[2] * vz[iat] ;

		// E cinetique
		ecinet[0] += 0.5 * masse * vx[iat] * vx[iat] ;
		ecinet[1] += 0.5 * masse * vy[iat] * vy[iat] ;
		ecinet[2] += 0.5 * masse * vz[iat] * vz[iat] ;

		// ajout de la vitesse globale initiale
		vx[iat] += vginitx ;
		vy[iat] += vginity ;
		vz[iat] += vginitz ;

		// vitesse centre de gravite
		vgx += masse * vx[iat] ;
		vgy += masse * vy[iat] ;
		vgz += masse * vz[iat] ;
	}

	vgx = vgx / massetot ;
	vgy = vgy / massetot ;
	vgz = vgz / massetot ;

	ektot = ecinet[0] + ecinet[1] + ecinet[2] ;
	temperature = 2. * ektot / ( 3. * (double) nat * boltzman ) ;

	return erreur ;
}

