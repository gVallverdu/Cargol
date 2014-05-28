
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * fichier main.h                                                            * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * librairies */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

/* * * * * dimensions */
#define	LEN	100
#define	NFLAG	27

/* * * * * definitions des structures */
#include "structures.h"

/* * * * * longueur du tableau de nbre aleatoires */
#define NRAND	75

/* * * * * nbre de pas pour echantillonner les temperature internes initiales */
#define NSTEPTEMP 10000

/* * * * * identificateurs des methodes */
#define idNVE		1
#define	idlangevin	2
#define	idLNVT		3
#define	idDPDnosplit	4
#define idDPDsplit	41
#define	idDPDE		5
#define idDPDE_alamain	51
#define idDPDE_ito	52


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * prototypes des fonctions recurentes                                       * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 
/* fonction stop */
extern void stop(char * ) ;

/* fonctions de tirage de nombres aleatoires */
extern double random_number( void ) ;
extern double ran0( long * ) ;
extern double ran1( long * ) ;
extern double ran2( long * ) ;
extern double gasdev( long * ) ;
extern double gauss( void ) ;
extern double gaussienne( void ) ;

