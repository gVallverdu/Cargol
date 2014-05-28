/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* construction de la liste des voisins symetrisee                           */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

int voisin( void ) {

	int 	erreur = 0 ;
	int	iat, jat ;
	double 	dist2 ;
	double	xij, yij, zij ;

	// boucle sur tous les atomes
	for ( iat = 0 ; iat < nat-1 ; iat++ ) {

		nbrevoisin[iat] = 0 ;

		// re-initialisation des deplacement des atomes
		dx[iat] = 0. ;
		dy[iat] = 0. ;
		dz[iat] = 0. ;

		// boucle sur les tous les atomes
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

			// distance iat - jat
			dist2 = xij * xij + yij * yij + zij * zij ;

			if ( dist2 < rverlet2 ) {
				// on a trouve un voisin
				listevoisin [iat] [ nbrevoisin[iat] ] = jat ;
				nbrevoisin[iat]++ ;
			
				if ( nbrevoisin[iat] == nbrevoisinmax + 1 ) {
					fprintf ( stdout, "\n\nnbrevoisin est trop petit !\n\n");
					exit(0) ;
				}
			}
		}
	}

	// pour le dernier atome	
	dx[nat-1] = 0. ;
	dy[nat-1] = 0. ;
	dz[nat-1] = 0. ;

	return erreur;
}

