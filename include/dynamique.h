
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * fichier entete de la fonction dynamique()                                 * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"
#include "DPDE.h"
#include "fichier.h"

// propagation de la dynamique hamiltonienne
extern int position( int ) ;
extern int force_ene( void ) ;
extern int vitesse( int ) ;

// tirage des nombres aleatoires
extern int tirage( void ) ;

// fonctions specifiques DPD et DPDE
extern int force_DPD( int, int ) ;
extern int force_DPDE( int ) ;

// liste de voisins et conditions periodiques
extern int voisin( void ) ;
extern int periodic_cond( void ) ;

// impressions et divers traitements
extern int moyenne( int ) ;
extern int print_conf( void ) ;
extern int print_vel( void ) ;
extern int print_radial_dist( void ) ;
extern int print_fin( int ) ;
extern int print_restart( void );
extern int free_memoire( void ) ;

// fonction locale : entete dynamique
static int entete( int ) ;
