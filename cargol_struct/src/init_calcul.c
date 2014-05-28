
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * initialisation du calcul                                                  * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"

extern int message2( Data * ) ;
extern int defaut( Data * ) ;
extern int lecture_input ( Data * , Etat * ) ;
extern int allocation ( int, Data * , Particules ** ) ;
extern int start_configuration( int , Data * , Particules ** ) ;
extern int start_vitesse( Data * , Etat *, Particules ** ) ;
extern int read_configuration( Data *, Etat *, Particules ** ) ;
extern int voisin( int , Data * , Particules ** ) ;
extern int force_ene( int , Data * , Etat * , Particules ** ) ;
extern int init_DPDE ( int , Data * , Particules ** ) ;
extern int print_data_run ( Data *, Etat * ) ;

int init_calcul ( Data *Donnees, Etat *EtatThermo, Particules *Particule[] ) {

  int     erreur = EXIT_SUCCESS ;
  int     ix ;
  double  duree ;
  char    madate[LEN] ;
  time_t  temps ;

  message2( Donnees ) ;

/* date de depart */  
  temps = time(NULL) ;
  strftime( madate, sizeof(madate), "%a %H h %M m %S s", localtime( &temps ) ) ;
  fprintf( Donnees->cntrl, " * depart      : %s \n", madate );
	

/* enregistrement des valeurs par defaut */
  fprintf( Donnees->cntrl, " * initialisation des valeurs par defaut\n") ;
  defaut( Donnees ) ;


/* lecture de l'input */
  fprintf( Donnees->cntrl, " * lecture du fichier d'entre           : %s \n", Donnees->input ) ;
  lecture_input( Donnees, EtatThermo ) ;


/* allocation des tableaux nat = nombre d'atome */
  fprintf( Donnees->cntrl, " * allocation de la memoire\n") ;
  allocation( EtatThermo->nat, Donnees, Particule ) ;
  

  if( strcmp( Donnees->restart, "non" ) == 0 ) {
/* construction de la configuration initiale */
    fprintf( Donnees->cntrl, " * construction de la configuration initiale\n") ;
    start_configuration( EtatThermo->nat, Donnees, Particule ) ;


/* vitesses initiales */
    fprintf ( Donnees->cntrl, " * calcul des vitesses initiales\n") ;
    start_vitesse( Donnees , EtatThermo, Particule ) ;
    

  } else {
/* lecture de la configuration sur le fichier restart */
    fprintf( Donnees->cntrl, " * lecture des posisitions et des vitesses sur le fichier :") ;
    fprintf( Donnees->cntrl, "    %s\n", Donnees->restart ) ;
    read_configuration( Donnees, EtatThermo, Particule ) ;
  }


/* initialisation de la liste des voisins */
  fprintf( Donnees->cntrl, " * construction des listes de voisins initiales\n") ;
  voisin( EtatThermo->nat, Donnees, Particule ) ;


/* calcul des forces de la configuration initiale (conservatives uniquement) */
  fprintf( Donnees->cntrl, " * calcul des forces initiales \n" ) ;
  force_ene( idNVE, Donnees, EtatThermo, Particule ) ;


/* remise a zero de g(r) apres le passage dans force_ene */
  for ( ix = 0 ; ix < Donnees->npas_radial_dist ; ix++ ) Donnees->radial_dist[ix] = 0. ;


/* initialisation de la temperature et de l'energie interne des particules */
  if( strstr( Donnees->methode, "DPDE" ) != NULL ) {
    fprintf( Donnees->cntrl," * initialisation des temperatures et energies internes initiales\n");
    init_DPDE ( EtatThermo->nat, Donnees, Particule ) ;
  }


/* impression des parametres du calcul */
  print_data_run( Donnees, EtatThermo ) ;

  Donnees->t_init = clock() ;
  duree = ( (double) (Donnees->t_init - Donnees->start) ) / CLOCKS_PER_SEC ;
  printf("\n * duree initialisation : %f s\n", duree );

  return erreur ;
}

