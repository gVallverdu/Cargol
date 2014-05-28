/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* calcul des forces et de l'energie totale pour un potentiel Lennard Jones  */
/* sans condition periodique aux limites                                     */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

int force_ene( void ) {

	int 	erreur = 0 ;

	int	iat, jat ;
	double	xij, yij, zij ;
	double	dist2 ;
	double 	dist_inv2, dist_inv6 ;
	double	coef, df, ene ;


	// initialisation energie potentielle
	eptot = 0. ;

	// initialisation des forces 
	for ( iat = 0 ; iat < nat ; iat++ ) {
		frcx[iat] = 0. ;
		frcy[iat] = 0. ;
		frcz[iat] = 0. ;
	}
	
	for ( iat = 0 ; iat < nat-1 ; iat++ ) {

		for ( jat = iat+1 ; jat < nat ; jat++ ) {

			// vecteur iat -> jat
			xij = x[jat] - x[iat] ;
			yij = y[jat] - y[iat] ;
			zij = z[jat] - z[iat] ;

			// distance iat - jat
			dist2 = xij * xij + yij * yij + zij * zij ;

			if ( dist2 < rcut2 ) {

				dist_inv2 = 1. / dist2 ;
				dist_inv6 = dist_inv2 * dist_inv2 * dist_inv2 ;

				// energie
				coef = LJ4eps * dist_inv6 * LJsigma6 ;

				ene = coef * ( LJsigma6 * dist_inv6 - 1.0 ) - LJshift ;

				// forces sur iat et jat, on calcule les forces = - grad E
	
				// derivee par rapport a dist
				// derivee par rapport aux coordonnées cartesiennes

				df = coef * ( 6. - 12. * LJsigma6 * dist_inv6 ) ;
				df = df * dist_inv2 ;

				frcx[iat] = frcx[iat] + df * xij ;
				frcy[iat] = frcy[iat] + df * yij ;
				frcz[iat] = frcz[iat] + df * zij ;

				frcx[jat] = frcx[jat] - df * xij ;
				frcy[jat] = frcy[jat] - df * yij ;
				frcz[jat] = frcz[jat] - df * zij ;

				eptot = eptot + ene ;
			}
		}
	}

	return erreur;
}

