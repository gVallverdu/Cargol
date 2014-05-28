
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * dynamique classique DPDE                                                  * 
 *                                                                           * 
 * algorithme de verlet vitesse pour la dynamique de newton                  * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"
#include "DPDE.h"

// propagation de la dynamique
extern int positionNVE( void ) ;
extern int force_ene( int ) ;
extern int force_DPDE( int ) ;
extern int vitesseNVE( void ) ;

// liste de voisins et conditions periodiques
extern int voisin( void ) ;
extern int periodic_cond( int ) ;

// impressions et divers traitements
extern int moyenne( int , int ) ;
extern int print_conf( int ) ;
extern int print_vel( int ) ;
extern int print_radial_dist( void ) ;
extern int print_fin( int ) ;
extern int print_restart( void );
extern int free_memoire( void ) ;

// entete dynamique
static int entete( void ) ;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int dynamique_DPDE( void ) {

  int   erreur = 0 ;
  int   step ;
  FILE *mdout ;
	
  // impression initiale
  entete() ;
  moyenne( 0, idDPDE ) ;

  // boucle globale sur le pas de temps
  for ( step = 1 ; step <= nstep ; step++ ) {

    // ajout des forces DPD + calcul energie interne
    force_DPDE( step ) ;

    // mise a jour des positions et 1/2 pas vitesse
    positionNVE() ;

    // conditions periodiques aux limites
    periodic_cond( step ) ;

    // mise a jour des voisin pour le calcul des forces
    if ( majvoisin > 0 ) {
      mdout = fopen( amdout, "a" ) ;
      fprintf ( mdout, "# %8d  * * * Mise a jour de la liste des voisins * * *\n", step) ;
      fclose(mdout) ; 

      voisin() ;
    }

    // calcul des forces et de l'energie dans le cas NVE
    force_ene( idNVE ) ;

    // dernier 1/2 pas vitesse
    vitesseNVE() ;

    // impressions ou calculs divers
    if ( nave != 0 && step % nave == 0 ) moyenne( step, idDPDE );

    // impression conf
    if ( ncrd != 0 && step % ncrd == 0 ) print_conf( step ) ;

    // impression vitesse
    if ( nvel != 0 && step % nvel == 0 ) print_vel ( step ) ;
  }

  // impression finale
  print_radial_dist() ;
  print_fin( idDPDE ) ;
  print_restart();
   	// liberation memoire
  free_memoire() ;

  return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * entete dynamique                                                          * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int entete ( void ) {

  int     erreur = 0 ;
  double  sigma ;
  FILE 	 *cntrl , *fT_int ;

  fprintf(stdout,"\n-----------------------------------------------------------------\n" );
  fprintf(stdout," * dynamique %s : \n", methode ) ;
  fprintf(stdout,"-----------------------------------------------------------------\n\n" );

  cntrl = fopen( acntrl , "a" ) ;
  fprintf( cntrl,"\n-----------------------------------------------------------------\n" );
  fprintf( cntrl," * dynamique %s : \n", methode ) ;
  fprintf( cntrl,"-----------------------------------------------------------------\n\n" );
  fprintf( cntrl, "resultats : %s \n", amdout ) ;

  sigma = sqrt( 2. * xsi * boltzman * temp0 / dt ) ;
  fprintf( cntrl,"\nfriction, xsi                      : %e kg.s-1\n",xsi ) ;
  fprintf( cntrl,  "amplitude du terme aleatoire       : %e N\n", sigma ) ;
  fprintf( cntrl,  "coefficient calorimetrique      Cv : %e J.K-1\n", capacite_calo ) ;
  fprintf( cntrl,  "temperature interne cible initiale : %e K \n", temp_interne0 ) ;

  fclose( cntrl ) ;

  fT_int = fopen( "temp_interne.dat", "w" ) ;
  fprintf( fT_int, "# iat     temp_interne     energie interne\n") ;
  fclose ( fT_int ) ;

  return erreur ;
}

