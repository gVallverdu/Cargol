/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* calcul des forces et de l'energie totale pour un potentiel Lennard Jones  */
/*                                                                           */
/* le calcul prend en compte les conditions periodiques aux limites          */
/* on cherche l'image de jat la plus proche de iat                           */
/* le calcul des forces est symetrise                                        */
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
	
	// boucles sur toutes les paires d'atomes
	for ( iat = 0 ; iat < nat-1 ; iat++ ) {

		for ( jat = iat+1 ; jat < nat ; jat++ ) {

			// vecteur iat -> jat
			xij = x[jat] - x[iat] ;
			yij = y[jat] - y[iat] ;
			zij = z[jat] - z[iat] ;

			// image de jat la plus proche de iat
			if ( xij >  boxx / 2. ) xij = xij - boxx ;
			if ( xij < -boxx / 2. ) xij = xij + boxx ;

			if ( yij >  boxy / 2. ) yij = yij - boxy ;
			if ( yij < -boxy / 2. ) yij = yij + boxy ;

			if ( zij >  boxz / 2. ) zij = zij - boxz ;
			if ( zij < -boxz / 2. ) zij = zij + boxz ;

			// distance iat - jat au carre
			dist2 = xij * xij + yij * yij + zij * zij ;

			if ( dist2 < rcut2 ) {

				dist_inv2 = 1. / dist2 ;
				dist_inv6 = dist_inv2 * dist_inv2 * dist_inv2 ;

				// energie
				coef = LJ4eps * dist_inv6 * LJsigma6 ;

				ene = coef * ( LJsigma6 * dist_inv6 - 1.0 ) - LJshift ;
				
				// forces sur iat et jat, on calcule les forces = - grad E
				// derivee par rapport a dist puis aux coordonnees xyz

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

