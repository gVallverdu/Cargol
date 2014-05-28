/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* Construction de la configuration initiale                                 */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

int start_configuration( void ) {

	int	erreur = 0 ;
	int	i, j, k, l, iat ;
	int	natm = 0;
	double	xi[4], yi[4], zi[4] ;
	FILE * inpcrd ;

	// position maille elementaire
	xi[0] = 0. ;
	yi[0] = 0. ;
	zi[0] = 0. ;

	if         ( strcmp( cristal, "simple" ) == 0 ) {
		// maille initiale simple
		natm = 1 ;

	} else if  ( strcmp( cristal, "centre" ) == 0 ) {
		// maille initiale centre
		natm = 2 ;
		xi[1] = 0.5 ;
		yi[1] = 0.5 ;
		zi[1] = 0.5 ;

	} else if  ( strcmp( cristal, "CFC" ) == 0 ) {
		// maille initiale CFC
		natm = 4 ;
		xi[1] = 0.  ;
		yi[1] = 0.5 ;
		zi[1] = 0.5 ;

		xi[2] = 0.5 ;
		yi[2] = 0.  ;
		zi[2] = 0.5 ;

		xi[3] = 0.5 ;
		yi[3] = 0.5 ;
		zi[3] = 0.  ;

	}

	// construction de la configuration initiale
	iat = 0 ;
	for ( i = 0 ; i < Nx ; i++) {
		for ( j = 0 ; j < Ny ; j++) {
			for ( k = 0 ; k < Nz ; k++) {
				for ( l = 0 ; l < natm ; l++ ) {
					x[iat] = ( xi[l] + (double) i ) * a0 ;
					y[iat] = ( yi[l] + (double) j ) * a0 ;
					z[iat] = ( zi[l] + (double) k ) * a0 ;
					
					iat++ ;
				}
			}
		}
	}

	// nombre d'atome total
	if ( iat != nat ) {
		fprintf( stdout, "\n\n ERREUR : calcul du nombre d'atome\n\n") ;
		exit(0) ;
	}

	// impression de la configuration intiale au format xyz
	inpcrd = fopen( "inpcrd.xyz", "w" ) ;

	fprintf( inpcrd, "%d\n", nat ) ;
	fprintf( inpcrd, "configuration initiale en angstrom : maille %s \n", cristal) ;
	for ( iat = 0 ; iat < nat ; iat++ ) {
		fprintf( inpcrd, "%s %8.3f %8.3f %8.3f \n", 
		aname, x[iat]*1e10, y[iat]*1e10, z[iat]*1e10 ) ;
	}
	fclose( inpcrd ) ;

	return erreur ;
}

