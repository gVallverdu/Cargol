/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* init_calcul.c                                                             */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"

// declaration des variables globales
#include "variables_globales.h"

// nom du fichier de donnees
extern	char	input[LEN] ;

// date initiale
extern	clock_t	t_init, start ;

// fonctions externes
extern	int	start_configuration( void ) ;
extern	int	start_vitesse( void ) ;
extern	int	read_configuration( void ) ;
extern	int	voisin ( void ) ;
extern	int	force_ene( int ) ;
extern	int	print_data_run ( void ) ;

// fonctions inclues dans ce fichier :
static	int	defaut ( void ) ;
static	int	lecture_input ( void ) ;
static	int	initialisation (void ) ;
static	int	allocation ( void ) ;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* initialisation du calcul                                                  */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// int init_calcul ( void ) <<<
int init_calcul ( void ) {

	int	erreur = 0 ;
	int 	ix ;
	double 	duree ;
	FILE * 	cntrl ;
	
	cntrl = fopen( acntrl, "a" ) ;

	// valeur par defaut et 27 vecteur translation
	fprintf ( cntrl, " * initialisation des valeurs par defaut\n") ;
	defaut() ;

	// lecture de l'input
	fprintf ( cntrl, " * lecture de du fichier d'entre           : %s \n", input ) ;
	lecture_input() ;

	// calculs divers en fonctions des donnees lues sur l'input : nat, volume ...
	initialisation () ;

	// allocation des tableaux nat = nombre d'atome
	fprintf ( cntrl, " * allocation de la memoire\n") ;
	allocation() ;

	if ( strcmp( restart, "non" ) == 0 ) {
		// construction de la configuration initiale
		fprintf ( cntrl, " * construction de la configuration initiale\n") ;
		start_configuration();

		// vitesses initiales
		fprintf ( cntrl, " * calcul des vitesses initiales\n") ;
		start_vitesse();

	} else {
		// lecture de la configuration sur le fichier restart
		fprintf ( cntrl, " * lecture des posisitions et des vitesses sur le fichier :") ;
		fprintf ( cntrl, "    %s\n", restart ) ;
		read_configuration() ;
	}

	// initialisation de la liste des voisins
	fprintf ( cntrl, " * construction des listes de voisins initiales\n") ;
	voisin();

	// calcul des forces de la configuration initiale (conservatives uniquement)
	fprintf ( cntrl, " * calcul des forces initiales \n" ) ;
	force_ene( 1 ) ;

	// remise a zero de g(r) après le passage dans force_ene
	for ( ix = 0 ; ix < npas_radial_dist ; ix++ ) {
		radial_dist[ix] = 0. ;
	}

	fclose ( cntrl ) ;

	// impression des parametres du calcul
	print_data_run() ;

	t_init = clock() ;
	duree = ( (double) (t_init - start) ) / CLOCKS_PER_SEC ;
	fprintf ( stdout,"\n * duree initialisation : %f s\n", duree );

	return erreur ;
}
// >>>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* Calculs divers : volume, nat, masse volumique ...                         */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// int initialisation ( void ) <<<
int initialisation ( void ) {

	int erreur = 0 ;
	double	r6 ;
	float	tempx, tempy, tempz; 
	FILE* mdout ;
	FILE* frestart ;

	// ouverture du fichier output -> s'il existe il est ecrase !
	mdout = fopen ( amdout, "w" ) ;
	fprintf ( mdout, "#  1 : step \n") ;
	fprintf ( mdout, "#  2 : energie totale par particule (J) \n") ;
	fprintf ( mdout, "#  3 : energie cinetique par particule (J) \n") ;
	fprintf ( mdout, "#  4 : energie potentielle par particule (J) \n") ;
	fprintf ( mdout, "#  5 : temperature  (K) \n") ;
	fprintf ( mdout, "#  6 : pression  (Pa) \n") ;
	fprintf ( mdout, "#  7 : vitesse du centre de gravite sur x (m.s-1) \n") ;
	fprintf ( mdout, "#  8 : vitesse du centre de gravite sur y (m.s-1) \n") ;
	fprintf ( mdout, "#  9 : vitesse du centre de gravite sur z (m.s-1) \n") ;
	fclose( mdout ) ;

	// calcul du nombre d'atome total
	if ( strcmp( restart, "non" ) != 0 ) {
		// nat lu sur le fichier d'entree
		frestart = fopen ( restart, "r") ;
		fscanf( frestart,"%d \n", &nat ) ;
		// dimension lue sur le fichier d'entree
		fscanf( frestart,"# dimension : %e, %e, %e \n", &tempx, &tempy, &tempz) ;
		fclose( frestart );

		boxx = (double) tempx ;
		boxy = (double) tempy ;
		boxz = (double) tempz ;

	} else if ( strcmp( cristal, "simple" ) == 0 ) {
		nat = 1 * Nx * Ny * Nz ;

	} else if  ( strcmp( cristal, "centre" ) == 0 ) {
		nat = 2 * Nx * Ny * Nz ;

	} else if  ( strcmp( cristal, "CFC" ) == 0 ) {
		nat = 4 * Nx * Ny * Nz ;
	}

	// masse
	masse = masse / avogadro ;

	// dimension de la boite 
	if ( strcmp( restart, "non" ) == 0 ) {
		boxx   = (double) Nx * a0 ;
		boxy   = (double) Ny * a0 ;
		boxz   = (double) Nz * a0 ;
	}
	if ( rcut > boxx / 2. || rcut > boxy / 2. || rcut > boxz / 2. ) {
		fprintf ( stdout, "\n\n rcut > dimension de la boite / 2.\n\n" );
		exit(0) ;
	}
	
	// calcul du volume
	volume = boxx * boxy * boxz ;

	// masse volumique
	mvol = (double) nat * masse / volume ;

	// parametre LJ
	LJ4eps		= 4. * LJeps ;
	LJsigma6	= pow( LJsigma , 6. ) ;
	r6		= pow( rcut , 6. ) ;
	LJshift		= LJ4eps * ( LJsigma6 / r6 )  * ( LJsigma6 / r6 - 1. ) ;

	// sauvegarde de rcut2 et rverlet2
	rcut2 = rcut * rcut ;
	rverlet2 = rverlet * rverlet ;

	// distribution radiale
	npas_radial_dist = (int) ( rcut / pas_radial_dist ) + 1 ;

	// deplacement max pour la liste des voisins
	dmax2 = 0.25 * (rcut - rverlet) * (rcut - rverlet) ;

	// graine pour le random_number
	srand( graine ) ;

	// valeurs moyennes
	ave_temp = 0. ;
	sig_temp = 0. ;
	ave_pres = 0. ;
	sig_pres = 0. ;
	ave_etot = 0. ;
	sig_etot = 0. ;
	ave_ektot = 0. ;
	sig_ektot = 0. ;
	ave_eptot = 0. ;
	sig_eptot = 0. ;

	return erreur ;
}
// >>>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* lecture du fichier input                                                  */
/*                                                                           */
/* test des valeurs lues                                                     */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// int lecture_input( void ) <<<
int lecture_input( void ) {

	int erreur = 0 ;

	int 	i , existe ;
	char	ligne[ LEN ] = "" ;
	char	poubelle[ LEN ] = "" ;
	float	tempx, tempy, tempz ;
	FILE * 	ftest ;
	FILE * 	finput ;

	// structure namelist
	struct namelist {
		char	flag[NFLAG][100] ;
		char	valeur[NFLAG][100] ;
		int	lu[NFLAG] ;
	} ;
	struct namelist	maliste ;

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *//
	//		initialisation de la namelist				//
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *//

	// liste des flags de la nameliste
	// parametres specifique au calcul
	strcpy( maliste.flag[ 0] , "methode" ) ;
	strcpy( maliste.flag[ 1] , "nstep" ) ;
	strcpy( maliste.flag[ 2] , "dt" ) ;
	strcpy( maliste.flag[ 3] , "xsi" ) ;
	strcpy( maliste.flag[ 4] , "temp0" ) ;
	strcpy( maliste.flag[ 5] , "rcut" ) ;
	strcpy( maliste.flag[ 6] , "rverlet" ) ; 
	// nombre aleatoires
	strcpy( maliste.flag[ 7] , "graine" ) ;
	// parametre du potentiel et des atomes
	strcpy( maliste.flag[ 8] , "aname" ) ;
	strcpy( maliste.flag[ 9] , "masse" ) ;
	strcpy( maliste.flag[10] , "LJsigma" ) ;
	strcpy( maliste.flag[11] , "LJeps" ) ;
	// parametres geometries initiales
	strcpy( maliste.flag[12] , "cristal" ) ;
	strcpy( maliste.flag[13] , "a0" ) ;
	strcpy( maliste.flag[14] , "dimension" ) ;
	// voisin
	strcpy( maliste.flag[15] , "nbrevoisinmax" ) ;
	// frequence output
	strcpy( maliste.flag[16] , "nave" ) ;
	strcpy( maliste.flag[17] , "ncrd" ) ;
	strcpy( maliste.flag[18] , "nout" ) ;
	strcpy( maliste.flag[19] , "nvel" ) ;
	// pas en R pour le calcul de g(r)
	strcpy( maliste.flag[20] , "pas_radial_dist" ) ;
	// restart
	strcpy( maliste.flag[21] , "restart" ) ;
	// vitesse globale initiale
	strcpy( maliste.flag[22] , "vg_initiale" ) ;
	// Cv pour la DPDE
	strcpy( maliste.flag[23] , "capacite_calo" ) ;
	

	// pour savoir si le flag a ete lu ou pas
	for ( i = 0 ; i < NFLAG ; i++ ) maliste.lu[ i] = 0 ; 

	// fichier contenant la namelist
	finput = fopen ( input , "r" ) ;

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *//


	// test existance du fichier input
	if ( finput == NULL ) {
		fprintf ( stdout, "\n\n FICHIER D'INPUT N'EXISTE PAS   : %s \n",input ) ;
		exit(0) ;
	}

	// lecture du fichier input qui contient la namelist
	while ( fgets( ligne, LEN, finput ) != NULL ) {

		// test ligne blanche
		i = 0 ;
		while ( ligne[i] != '\n' && ( ligne[i] == ' ' || ligne[i] == '\t' ) ) {
			++i ;
		}
		if ( ligne[i] == '\n' ) continue ;
 
		// si c'est un commentaire
		if ( strchr( &ligne[0], '#' ) != NULL || 
		     strchr( &ligne[0], '*' ) != NULL ) continue ; 

		// identification du flag et lecture de la valeur
		existe = 0 ;
		for ( i = 0 ; i < NFLAG ; i++ ) {
			if ( strstr( ligne, maliste.flag[i] ) != NULL ) {
				// on a identifie un flag 
				if ( maliste.lu[i] == 1 ) {
					fprintf ( stdout, 
					  "\n\n le flag %s apparait plusieurs fois \n\n", 
					  maliste.flag[i] );

					exit(0) ;
				} else {
					maliste.lu[i] = 1 ;
					existe = 1 ;
					sscanf ( ligne , "%s %s", poubelle, maliste.valeur[i] ) ;
					break ;
				}
			}
		}

		// fprintf ( stdout, "valeur : %s \n", maliste.valeur[i] ) ;

		if ( existe == 0 ) {
			fprintf ( stdout, 
			  "\n\nErreur sur cette ligne du fichier %s : %s \n", input, ligne) ;
			fprintf ( stdout, "flag inconnu !\n\n" ) ;
			exit(0) ;
		}
        }

	fclose( finput ) ;

	// si un flag a ete lu on attribue la valeur a la variables globales correspondante
	if ( maliste.lu[0] == 1 ) {
		strcpy( methode, maliste.valeur[0] ) ;
		if ( strcmp( methode, "brownien" ) != 0 && 
		     strcmp( methode, "NVE" )      != 0 &&
		     strcmp( methode, "langevin" ) != 0 &&
		     strcmp( methode, "LNVT" )     != 0 &&
		     strcmp( methode, "DPD" )      != 0 &&
		     strcmp( methode, "DPDE" )     != 0 ) {
			fprintf ( stdout, "\n\nType de calcul inconnu : %s \n\n", methode ) ;
			exit(0) ;
		}
	}

	if ( maliste.lu[1] == 1 ) {
		nstep = atoi ( maliste.valeur[1] ) ;
	}

	if ( maliste.lu[2] == 1 ) {
		dt = atof ( maliste.valeur[2] ) ;
	}

	if ( maliste.lu[3] == 1 ) {
		xsi = atof ( maliste.valeur[3] ) ;
	}

	if ( maliste.lu[4] == 1 ) {
		temp0 = atof ( maliste.valeur[4] ) ;
	}

	if ( maliste.lu[5] == 1 ) {
		rcut = atof ( maliste.valeur[5] ) ;
	}

	if ( maliste.lu[6] == 1 ) {
		rverlet = atof ( maliste.valeur[6] ) ;
	}

	if ( maliste.lu[7] == 1 ) {
		graine = atoi ( maliste.valeur[7] ) ;
	}

	if ( maliste.lu[8] == 1 ) {
		strcpy( aname, maliste.valeur[8] ) ;
	}

	if ( maliste.lu[9] == 1 ) {
		masse = atof ( maliste.valeur[9] ) ;
	}

	if ( maliste.lu[10] == 1 ) {
		LJsigma = atof ( maliste.valeur[10] ) ;
	}

	if ( maliste.lu[11] == 1 ) {
		LJeps = atof ( maliste.valeur[11] ) ;
	}

	if ( maliste.lu[12] == 1 ) {
		strcpy( cristal, maliste.valeur[12] ) ;
		if ( strcmp( cristal, "simple" ) != 0 && 
		     strcmp( cristal, "centre" ) != 0 &&
		     strcmp( cristal, "CFC" )    != 0 ) {
			fprintf ( stdout, "\n\nMaille elementaire inconnu : %s \n\n", cristal ) ;
			exit(0) ;
		}
	}

	if ( maliste.lu[13] == 1 ) {
		a0 = atof ( maliste.valeur[13] ) ;
	}

	if ( maliste.lu[14] == 1 ) {
		sscanf ( maliste.valeur[14] , "%d , %d , %d", &Nx, &Ny, &Nz ) ;
		if ( Nx == 0 || Ny == 0 || Nz == 0 ) {
			fprintf ( stdout, 
			  "\n\nLes dimensions doivent etre > ou = a 1 :	%d, %d, %d \n\n", 
			  Nx, Ny, Nz ) ;
			exit(0) ;
		}
	}

	if ( maliste.lu[15] == 1 ) {
		nbrevoisinmax = atoi ( maliste.valeur[15] ) ;
	}

	if ( maliste.lu[16] == 1 ) {
		nave = atoi ( maliste.valeur[16] ) ;
	}

	if ( maliste.lu[17] == 1 ) {
		ncrd = atoi ( maliste.valeur[17] ) ;
	}

	if ( maliste.lu[18] == 1 ) {
		nout = atoi ( maliste.valeur[18] ) ;
	}

	if ( maliste.lu[19] == 1 ) {
		nvel = atoi ( maliste.valeur[19] ) ;
	}

	if ( maliste.lu[20] == 1 ) {
		pas_radial_dist = atof ( maliste.valeur[20] ) ;
	}

	if ( maliste.lu[21] == 1 ) {
		strcpy( restart, maliste.valeur[21] ) ;
		if (  strcmp( restart, "non" ) != 0 ) {
			ftest = fopen( restart , "r" ) ;
			if ( ftest == NULL ) {
				fprintf( stdout, "\n\n fichier restart introuvable :  %s\n\n", restart);
				exit(0) ;
			} else {
				fclose ( ftest ) ;
			}
		}
	}

	if ( maliste.lu[22] == 1 ) {
		sscanf ( maliste.valeur[22] , "%f , %f , %f ", &tempx, &tempy, &tempz ) ;
		vginitx = (double) tempx ;
		vginity = (double) tempy ;
		vginitz = (double) tempz ;
	}

	if ( maliste.lu[23] == 1 ) {
		capacite_calo = atof ( maliste.valeur[23] ) ;
	}

	// warnings
	if ( rcut < 2.5 * LJsigma )   fprintf ( stdout, "\n\nAttention rcut < 2.5 * LJsigma \n\n") ;

	return erreur ;
}
// >>>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* allocation des tableaux dynamique : forces, positions, vitesses           */
/*                                                                           */
/* initialistion a 0 de tous les tableaux                                    */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// int allocation( void ) <<<
int allocation( void ) {

	int erreur = 0 ;
	int iat, ivois, ix, jat ;

	// forces
	frcx = calloc( nat, sizeof(double) ) ;
	frcy = calloc( nat, sizeof(double) ) ;
	frcz = calloc( nat, sizeof(double) ) ;
	
	// positions
	x = calloc( nat, sizeof(double) ) ;
	y = calloc( nat, sizeof(double) ) ;
	z = calloc( nat, sizeof(double) ) ;
	
	// vitesses
	vx = calloc( nat, sizeof(double) ) ;
	vy = calloc( nat, sizeof(double) ) ;
	vz = calloc( nat, sizeof(double) ) ;

	// deplacement
	dx = calloc( nat, sizeof(double) ) ;
	dy = calloc( nat, sizeof(double) ) ;
	dz = calloc( nat, sizeof(double) ) ;

	// energie interne
	e_interne = calloc( nat, sizeof(double) );

	// temperature interne
	temp_interne = calloc( nat, sizeof(double) );

	// liste de voisins
	listevoisin = calloc( nat, sizeof(int*) ) ;
	for ( iat = 0 ; iat < nat ; iat++ ) {
		listevoisin[iat] = calloc( nbrevoisinmax, sizeof(int) ) ;
	}

	// nbre de voisins de chaque atome
	nbrevoisin = calloc( nat, sizeof(int) ) ;

	// distribution radiale
	radial_dist = calloc( npas_radial_dist, sizeof(double) ) ;

	// nombres aleatoires
	/*
	wxrand = calloc( nat, sizeof(double) ) ;
	wyrand = calloc( nat, sizeof(double) ) ;
	wzrand = calloc( nat, sizeof(double) ) ;
	for ( iat = 0 ; iat < nat ; iat++ ) {
		wxrand[iat] = calloc( nat, sizeof(double) ) ;
		wyrand[iat] = calloc( nat, sizeof(double) ) ;
		wzrand[iat] = calloc( nat, sizeof(double) ) ;
	}
	*/

	for ( ix = 0 ; ix < npas_radial_dist ; ix++ ) {
		radial_dist[ix] = 0. ;
	}

	// initialisation a 0 de tous les tableaux
	for ( iat = 0 ; iat < nat ; iat++ ) {
		frcx[iat] = 0. ;
		frcy[iat] = 0. ;
		frcz[iat] = 0. ;

		x[iat] = 0. ;
		y[iat] = 0. ;
		z[iat] = 0. ;

		vx[iat] = 0. ;
		vy[iat] = 0. ;
		vz[iat] = 0. ;

		e_interne[iat] = 0. ;
		temp_interne[iat] = 0. ;

		nbrevoisin[iat] = 0 ;

		/*
		for ( jat = 0 ; jat < nat ; jat++ ) {
			wxrand[iat][jat] = 0. ;
			wyrand[iat][jat] = 0. ;
			wzrand[iat][jat] = 0. ;
		}
		*/

		for ( ivois = 0 ; ivois < nbrevoisinmax ; ivois++ ) {
			listevoisin [iat] [ivois] = 0 ;
		}
	}

	return erreur ;
}
// >>>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* valeur par defaut de tous les parametres                                  */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// int defaut( void ) <<<
int defaut( void ) {

	int erreur = 0 ;

	// constantes
	boltzman  	= 1.380662e-23 ;  	// J.K-1
	avogadro 	= 6.0228e23 ;		// mol-1
	qelectron 	= 1.6021892e-19 ;	// C
	aBohr 		= 0.5291e-10 ;		// m
	pi 		= 4. * atan(1.) ;	

	// parametres specifique au calcul
	strcpy ( methode ,"NVE" ) ;
	nstep		= 1000 ;
	dt		= 0.001e-12 ;		// s
	xsi 		= 1e-12 ;		// kg.s-1
	nave		= 5 ;
	ncrd		= 200 ;
	nout		= 1000 ;
	nvel		= 0 ;

	// geometry initiale 
	strcpy ( cristal, "simple" ) ;
	a0		= 5.0e-10 ;		// m
	Nx		= 10 ;
	Ny		= 10 ;
	Nz		= 10 ;
	strcpy( restart, "non" ) ;		// par defaut pas de restart

	// vitesse globale initiale
	vginitx = 0. ;
	vginity = 0. ;
	vginitz = 0. ;

	// distribution radiale
	pas_radial_dist	= 0.1e-10 ;		// m

	// nombres aleatoires
	graine		= 15041983 ;

	// pour les vitesses initiales
	temp0	 	= 300.e0 ;		// K

	// parametre du potentiel et des atomes
	strcpy ( aname , "Ar" ) ;
	masse		= 39.95e-3 ;		// kg.mol-1
	LJsigma		= 3.405e-10 ;		// m
	LJeps		= 1.6567944e-21 ;	// J

	// cut off
	rcut		= 2.5 * LJsigma ;	// m
	rverlet		= 3.0 * LJsigma ;	// m
	nbrevoisinmax	= 100 ;

	// DPDE
	capacite_calo 	= 1e-23 ;		// J.K-1

	return erreur ;
}
// >>>

