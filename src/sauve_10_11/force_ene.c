/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* calcul des forces et de l'energie potentielle totale                      */
/*                                                                           */
/* cas d'un potentiel Lennard Jones                                          */
/* le calcul prend en compte les conditions periodiques aux limites          */
/* on cherche l'image de jat la plus proche de iat                           */
/* on utilise le principe des listes de verlet                               */
/* le calcul des forces est symetrise car les listes sont symetrisees        */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

extern	int	force_DPD ( double, int, int ) ;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int force_ene( int idmethode ) {

	int 	erreur = 0 ;

	int	iat, jat, ivois, k, nvois ;
	double	xij, yij, zij ;
	double	dist2 ;
	double 	dist_inv2, dist_inv6 ;
	double	coef, df, ene ;
	double	fx, fy, fz ;

	// initialisation energie potentielle
	eptot = 0. ;

	// initialisation des forces 
	for ( iat = 0 ; iat < nat ; iat++ ) {
		frcx[iat] = 0. ;
		frcy[iat] = 0. ;
		frcz[iat] = 0. ;
	}

	// initialisation du viriel
	virielx = viriely = virielz = 0. ;
	
	// boucles sur toutes les paires d'atomes
	for ( iat = 0 ; iat < nat-1 ; iat++ ) {

		nvois = nbrevoisin[iat] ;
		for ( ivois = 0 ; ivois < nvois ; ivois++ ) {

			jat = listevoisin [iat] [ivois] ;

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

				eptot += ene ;
				
				// forces sur iat et jat, on calcule les forces = - grad E
				// derivee par rapport a dist puis aux coordonnees xyz

				df = coef * ( 6. - 12. * LJsigma6 * dist_inv6 ) ;
				df = df * dist_inv2 ;

				fx = df * xij ;
				fy = df * yij ;
				fz = df * zij ;

				frcx[iat] += fx ;
				frcy[iat] += fy ;
				frcz[iat] += fz ;

				frcx[jat] -= fx ;
				frcy[jat] -= fy ;
				frcz[jat] -= fz ;

				// calcul du viriel
				virielx += xij * fx ;
				viriely += yij * fy ;
				virielz += zij * fz ;

				// DPD
				if ( idmethode >= idDPD ) force_DPD( dist2, iat, jat ) ;

				// mise a jour de g(r)
				k = (int) ( sqrt(dist2) / pas_radial_dist ) ;
				radial_dist[k] += 2. ; // contribution iat + jat = 2
			}
		}
	}

	return erreur;
}

