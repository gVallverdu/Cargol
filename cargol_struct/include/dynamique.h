
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * fichier entete de la fonction dynamique()                                 * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"

// propagation de la dynamique hamiltonienne
extern int position( int , int , Data * , Particules ** ) ;
extern int force_ene( int , Data * , Etat * , Particules ** ) ;
extern int vitesse( int , int , Data * , Particules ** ) ;

// tirage des nombres aleatoires
extern int tirage( int , long * ) ;

// fonctions specifiques DPD et DPDE
extern int force_DPD( int, int, int , Data * , Particules ** ) ;
extern int force_DPDE( int, int , Data * , Particules ** ) ;

// liste de voisins et conditions periodiques
extern int voisin( int , Data * , Particules ** ) ;
extern int periodic_cond( int , Data * , Particules ** ) ;

// impressions et divers traitements
extern int moyenne( int , Data * , Etat * , Particules ** ) ;
extern int print_conf( int , Data * , Particules ** ) ;
extern int print_vel( int , Data * , Particules ** ) ;
extern int print_radial_dist( Data *, Etat * ) ;
extern int print_fin( int , Data * , Etat * , Particules ** ) ;
extern int print_restart( int , Data *, Particules ** );
extern int free_memoire( Data * ,Particules ** ) ;

// fonction locale : entete dynamique
static int entete( int, Data * ) ;
