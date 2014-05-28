/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* dynamique classique NVE                                                   */
/*                                                                           */
/* algorithme de verlet vitesse                                              */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

// propagation de la dynamique
extern	int	position( int ) ;
extern 	int	force_ene( int ) ;
extern	int	vitesse( int ) ;

// liste de voisins et conditions periodiques
extern	int	voisin( void ) ;
extern	int	periodic_cond( void ) ;

// impressions et divers traitements
extern	int	moyenne( int ) ;
extern	int	print_conf( int ) ;
extern	int	print_vel( int ) ;
extern	int	print_radial_dist( void ) ;
extern	int	print_fin( void ) ;
extern	int	print_restart( void );
extern	int	free_memoire( void ) ;

// entete dynamique
static	int	entete( int ) ;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int dynamique( int idmethode ) {

	int 	erreur = 0 ;
	int 	step ;
	FILE 	*mdout ;
	
	// impression initiale
	entete( idmethode ) ;
	moyenne( 1 ) ;

	// boucle globale sur le pas de temps
	for ( step = 1 ; step <= nstep ; step++ ) {

		// calcul des nouvelles positions
		// premier 1/2 pas vitesse
		position( idmethode ) ;

		// conditions periodiques aux limites
		periodic_cond() ;

		// mise a jour des voisin pour le calcul des forces
		if ( majvoisin > 0 ) {
			mdout = fopen( amdout, "a" ) ;
			fprintf ( mdout,
                        "# %8d  * * * Mise a jour de la liste des voisins * * *\n", step) ;
			fclose(mdout) ; 

			voisin() ;
		}

		// calcul des forces et de l'energie
		force_ene( idmethode ) ;

		// dernier 1/2 pas vitesse
		vitesse( idmethode ) ;

		// impressions ou calculs divers
		if ( nave != 0 && step % nave == 0 ) moyenne( step );

		// impression conf
		if ( ncrd != 0 && step % ncrd == 0 ) print_conf( step ) ;

		// impression vitesse
		if ( nvel != 0 && step % nvel == 0 ) print_vel ( step ) ;
	}

	// impression finale
	print_radial_dist() ;
	print_fin() ;
	print_restart();

	// liberation memoire
	free_memoire() ;

	return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* entete dynamique                                                          */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int entete ( int idmethode ) {

	int	erreur = 0 ;
	double	alpha, sigma, gamma ;
	FILE 	*cntrl ;

	fprintf(stdout,"\n-----------------------------------------------------------------\n" );
	fprintf(stdout," * dynamique %s : \n", methode ) ;
	fprintf(stdout,"-----------------------------------------------------------------\n\n" );

	cntrl = fopen( acntrl , "a" ) ;
	fprintf( cntrl,"\n-----------------------------------------------------------------\n" );
	fprintf( cntrl," * dynamique %s : \n", methode ) ;
	fprintf( cntrl,"-----------------------------------------------------------------\n\n" );
	fprintf( cntrl, "resultats : %s \n", amdout ) ;

	if ( idmethode == idlangevin ) {
		alpha = exp( - xsi * dt / masse ) ;
		sigma = sqrt( boltzman * temp0 / masse * ( 1. - alpha*alpha) ) ;
		fprintf( cntrl,"\nfriction, xsi                     : %e kg.s-1\n", xsi ) ;
		fprintf( cntrl,  "parametre langevin sans dimension : %e\n"      , alpha ) ;
		fprintf( cntrl,  "amplitude du terme aleatoire      : %e m.s-1\n", sigma ) ;

	} else if ( idmethode == idLNVT ) {
		gamma = 0.5 * xsi * dt ;
		sigma = sqrt( ( 1. + gamma ) * 2. * xsi * boltzman * temp0 * masse / dt ) ;
		fprintf( cntrl,"\nfriction, xsi                     : %e s-1\n", xsi ) ;
		fprintf( cntrl,  "parametre langevin sans dimension : %e\n"  , gamma ) ;
		fprintf( cntrl,  "amplitude du terme aleatoire      : %e N\n", sigma ) ;

	} else if ( idmethode == idDPD || idmethode == idDPDE ) {
		alpha = exp( - xsi * dt / masse ) ;
		sigma = sqrt( 2. * xsi * boltzman * temp0 / dt ) ;
		fprintf( cntrl,"\nfriction, xsi                     : %e kg.s-1\n",xsi ) ;
		fprintf( cntrl,  "parametre langevin sans dimension : %e\n"  , alpha ) ;
		fprintf( cntrl,  "amplitude du terme aleatoire      : %e N\n", sigma ) ;

	}

	fclose( cntrl ) ; 

	return erreur ;
}

