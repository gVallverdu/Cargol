
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * initialisation du calcul                                                  * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"
#include "fichier.h"

extern int message2( void ) ;
extern int defaut ( void ) ;
extern int lecture_input ( void ) ;
extern int allocation ( void ) ;
extern int start_configuration( void ) ;
extern int start_vitesse( void ) ;
extern int read_configuration( void ) ;
extern int voisin ( void ) ;
extern int force_ene( int ) ;
extern int init_DPDE ( void ) ;
extern int print_data_run ( void ) ;

int init_calcul ( void ) {

  int     erreur = EXIT_SUCCESS ;
  int     ix ;
  double  duree ;
  char    madate[LEN] ;
  time_t  temps ;
  
  // depart
  message2() ;

  // date de depart   
  temps = time(NULL) ;
  strftime ( madate, sizeof(madate), "%a %H h %M m %S s", localtime(&temps));
  fprintf(cntrl, " * depart      : %s \n", madate );

  // enregistrement des valeurs par defaut
  fprintf ( cntrl, " * initialisation des valeurs par defaut\n") ;
  defaut() ;

  // lecture de l'input
  fprintf ( cntrl, " * lecture du fichier d'entre           : %s \n", input ) ;
  lecture_input() ;

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
  force_ene( idNVE ) ;

  // remise a zero de g(r) apres le passage dans force_ene
  for ( ix = 0 ; ix < npas_radial_dist ; ix++ ) radial_dist[ix] = 0. ;

  // initialisation de la temperature et de l'energie interne des particules
  if ( strstr( methode, "DPDE" ) != NULL ) {
    fprintf (cntrl," * initialisation des temperatures et energies internes initiales\n");
    init_DPDE() ;
  }

  // impression des parametres du calcul
  print_data_run() ;

  t_init = clock() ;
  duree = ( (double) (t_init - start) ) / CLOCKS_PER_SEC ;
  printf("\n * duree initialisation : %f s\n", duree );

  return erreur ;
}

