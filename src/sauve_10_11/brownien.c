/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* mouvement brownien 2D simple                                              */
/*                                                                           */
/* xsi est utilise pour definir l'amplitude du pas aleatoire                 */
/*                                                                           */
/* on peut vérifier que la distribution des positions des particules à un    */
/* temps t donné est une gaussienne centrée sur 0 et de largeur              */
/* xsi * sqrt( nstep * t)                                                    */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

extern 	double	gaussienne(void) ;

int brownien( void ) {

	int 	erreur = 0 ;
	int 	iat, step ;
	double 	sigma ;
	double	stdevth = 0. ;
	double	moy = 0., stdev = 0.;

	FILE *sortie ;

	fprintf(stdout,"\n") ;
	fprintf(stdout," * Mouvement brownien 2D : \n") ;
	fprintf(stdout,"\n") ;
	fprintf(stdout,"Le fichier brown2D.traj contient la trajectoire d'un atome.\n") ;
	fprintf(stdout,"Le fichier brown2D_position_finale contient la coordonnée X\n") ;
	fprintf(stdout,"finale de tous les atomes.\n") ;
	fprintf(stdout,"\n") ;

	// origine des positions
	for ( iat = 0 ; iat < nat ; iat++ ) {
		x[iat] = 0. ;
		y[iat] = 0. ;
		z[iat] = 0. ;
	}

	// amplitude des pas
	sigma = xsi * sqrt(dt) ; 
	stdevth = xsi * sqrt ( dt * (double) nstep ) ;

	fprintf( stdout, "amplitude des pas aléatoires          : %e \n", sigma );
	fprintf( stdout, "largeur de la distribution des positions de l'ensemble\n") ;
	fprintf( stdout, "des particules attendue au bout de nstep pas\n") ;
	fprintf( stdout, "sigma * sqrt(n dt)                    :  %e \n",  stdevth ) ;
	fprintf( stdout, "\n" );

	sortie = fopen( "brown2D.traj", "w") ;
	for ( step = 0 ; step < nstep ; step++ ) {
		for ( iat = 0 ; iat < nat ; iat++ ) {
			x[iat] += sigma * gaussienne();
			y[iat] += sigma * gaussienne();
		}
		fprintf( sortie, "%e %e \n", x[0], y[0] );
	}
	fclose( sortie );
	
	sortie = fopen( "brown2D_position_finale", "w") ;
	for ( iat = 0 ; iat < nat ; iat++ ) {
		moy   = moy   + x[iat] ;
		stdev = stdev + x[iat] * x[iat] ;
		fprintf( sortie, "%d %e \n", iat, x[iat]);
	}
	fclose( sortie );
	moy   = moy   / (double) nat ;
	stdev = stdev / (double) nat ;
	stdev = sqrt( stdev - moy * moy ) ;

	fprintf( stdout, "distribution des positions finales sur l'axe X de tous les atomes\n") ;
	fprintf( stdout, "moyenne     : %e \n", moy) ;
	fprintf( stdout, "ecart type  : %e \n", stdev) ;
	fprintf( stdout, "\n") ;

	return erreur;
}

