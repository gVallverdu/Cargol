/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* liberation de la memoire de tous les tableaux                             */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

int free_memoire ( void ) {

	int erreur = 0 ;

	// forces
	free ( frcx );
	free ( frcy );
	free ( frcz );
	
	// positions
	free ( x );
	free ( y );
	free ( z );
	
	// vitesses
	free ( vx ) ;
	free ( vy ) ;
	free ( vz ) ;

	// déplacement
	free ( dx ) ;
	free ( dy ) ;
	free ( dz ) ;

	// liste de voisins
	free ( listevoisin );

	// nbre de voisins de chaque atome
	free ( nbrevoisin ) ;

	// distribution radiale
	free ( radial_dist ) ;

	return erreur ;
}

