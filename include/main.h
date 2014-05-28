
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * fichier main.h                                                            * 
 *                                                                           * 
 * contient les includes necessaires                                         * 
 * contient les definitions de parametres                                    * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// librairies
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

// dimensions
#define	LEN	100
#define	NFLAG	34

// longueur du tableau de nbre aleatoires
#define NRAND	75

// nbre de pas pour echantillonner les temperature
// internes initiales
#define NSTEPTEMP 10000

// id methodes
#define idNVE		1
#define	idlangevin	2
#define	idLNVT		3
#define	idDPDnosplit	4
#define idDPDsplit	41
#define	idDPDE		5
#define idDPDE_alamain	51
#define idDPDE_ito	52

// potentiels
#define LenardJones	1
#define Exp6		2
#define Morse		3

// structure namelist
struct namelist {
    char  flag[NFLAG][LEN] ;
    char  valeur[NFLAG][LEN] ;
    int	  lu[NFLAG] ;
} ;

// declaration des fonctions recurentes
extern void stop(char * ) ;

// tirages de nombres aleatoires
extern double random_number( void ) ;
extern double ran0( long * ) ;
extern double ran1( long * ) ;
extern double ran2( long * ) ;
extern double gasdev( long * ) ;
extern double gauss( void ) ;
extern double gaussienne( void ) ;
