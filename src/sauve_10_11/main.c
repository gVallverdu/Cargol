/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* programme princpal                                                        */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

// fonctions
extern 	int	init_calcul( void );
extern 	int	brownien( void );
extern 	int	dynamique( int );
static	int	message( void );

// nom de l'input
char	input[LEN] ;

// type de calcul
extern	char	methode[LEN] ;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

clock_t start, end, t_init ;

int main( int narg, char *arg[] ) {

	int 	erreur = 0 ;
	double	duree ;
	FILE * 	cntrl ;

	start = clock() ;

	if ( narg == 1 ) {
		fprintf( stdout, "\n\nSyntaxe : %s fichier_input \n\n", arg[0]) ;
		exit(0) ;
	} else if ( narg > 2 ) {
		fprintf( stdout, "\n\n * attention : un seul argument est attendu \n\n" ) ;
	}

	// message 
	message() ;

	// le fichier input est le deuxieme argument :
	strcpy( input, arg[1] ) ;

	// mouvement brownien
	if ( strcmp( methode, "brownien" ) == 0 ) {
		brownien() ; 

	} else {
		// Lecture de l'input et initialisation des variables :
		init_calcul();

		// dynamique NVE
		if ( strcmp( methode, "NVE"      ) == 0 ) dynamique( idNVE ) ; 

		// dynamique de langevin
		if ( strcmp( methode, "langevin" ) == 0 ) dynamique( idlangevin ) ; 
		if ( strcmp( methode, "LNVT"     ) == 0 ) dynamique( idLNVT ) ; 

		// DPD
		if ( strcmp( methode, "DPD"      ) == 0 ) dynamique( idDPD ) ; 

		// DPDE
		if ( strcmp( methode, "DPDE"     ) == 0 ) dynamique( idDPDE ) ; 
	}

	// affichage temps :
	end = clock() ;
	duree = ( (double) (end - t_init) ) / CLOCKS_PER_SEC ;
	cntrl = fopen( acntrl , "a" ) ;
	fprintf(cntrl," * temps d'execution : %f s\n", duree );
	fprintf(cntrl,"-----------------------------------------------------------------\n\n" );
	fclose( cntrl );
	fprintf(stdout, "\n\n-----------------------------------------------------------------\n" );
	fprintf(stdout," * temps d'execution : %f s\n", duree );
	fprintf(stdout,"-----------------------------------------------------------------\n\n" );


	return erreur;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* message d'accueil ;)                                                      */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int message ( void ) {

	int erreur = 0 ;
	char madate[LEN] ;
	time_t temps ;

	FILE * cntrl ;

	cntrl = fopen ( acntrl, "w" ) ;

	fprintf(cntrl,"\n\n  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n" );
	fprintf(cntrl,    "  *                                                             *\n" ); 
	fprintf(cntrl,    "  *                         cargol                              *\n" );
	fprintf(cntrl,    "  *                                                             *\n" ); 
	fprintf(cntrl,    "  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n\n" );

	fprintf(stdout,"\n\n  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n" );
	fprintf(stdout,    "  *                                                             *\n" ); 
	fprintf(stdout,    "  *                         cargol                              *\n" );
	fprintf(stdout,    "  *                                                             *\n" ); 
	fprintf(stdout,    "  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n\n");

	fprintf(cntrl,    "                               \\____       ________             \n" ); 
	fprintf(cntrl,    "                               /    \\     /  ____  \\           \n" ); 
	fprintf(cntrl,    "                               \\_    \\   /  /    \\  \\     -- \n" ); 
	fprintf(cntrl,    "                                 \\    \\ (   \\__/  )  )    --  \n" ); 
	fprintf(cntrl,    "                                  \\    \\_\\ \\_____/  /__    --\n" ); 
	fprintf(cntrl,    "                                   \\______\\________/___'=-   \n\n");

	temps = time(NULL) ;
	strftime ( madate, sizeof(madate), "%a %H h %M m %S s", localtime(&temps));
	fprintf(cntrl, "\n\n * depart      : %s \n", madate );

	fclose(cntrl) ;

	return erreur ;
}
