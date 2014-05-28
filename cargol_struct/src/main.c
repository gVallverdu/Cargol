
 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
  *                                                                           * 
  * programme princpal                                                        * 
  *                                                                           * 
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* librairies, define et structure */
#include "main.h"

extern int commande( int , char **, Data * ) ;
extern int init_fichier( void ) ;
extern int message1( void ) ;
extern int init_calcul( Data * , Etat *, Particules ** ) ;
extern int dynamique( int , Data * , Etat *, Particules ** ) ;
extern int fermeture( Data * ) ;

int main( int narg, char **arg ) {

  int erreur = EXIT_SUCCESS ;

/* Declaration des structures */
  Etat        EtatThermo ;	// alias = e
  Data        Donnees ;		// alias = d
  Particules  *Particule ;	// alias = p

/* initialisation des temps */
  Donnees.start = clock() ;

/* message d'accueil */
  message1() ;
  
/* gestion de la ligne de commande */
  commande( narg, arg, &Donnees ) ;

/* Lecture de l'input et initialisation des variables : */
  init_calcul( &Donnees, &EtatThermo, &Particule ) ;

/* dynamique NVE */
  if ( strcmp( Donnees.methode, "NVE"          ) == 0 ) 
  	dynamique( idNVE, &Donnees, &EtatThermo, &Particule ) ; 

/* dynamique de langevin */
  if ( strcmp( Donnees.methode, "langevin"     ) == 0 ) 
  	dynamique( idlangevin, &Donnees, &EtatThermo, &Particule ) ; 
  if ( strcmp( Donnees.methode, "LNVT"         ) == 0 ) 
  	dynamique( idLNVT, &Donnees, &EtatThermo, &Particule ) ; 

/* dynamique DPD */
  if ( strcmp( Donnees.methode, "DPD"          ) == 0 ) 
  	dynamique( idDPDsplit, &Donnees, &EtatThermo, &Particule ) ;
  if ( strcmp( Donnees.methode, "DPDnosplit"   ) == 0 ) 
  	dynamique( idDPDnosplit, &Donnees, &EtatThermo, &Particule ) ;

/* dynamique DPDE */
  if ( strcmp( Donnees.methode, "DPDE"         ) == 0 ) 
  	dynamique( idDPDE, &Donnees, &EtatThermo, &Particule ) ; 
  if ( strcmp( Donnees.methode, "DPDE_alamain" ) == 0 ) 
  	dynamique( idDPDE_alamain, &Donnees, &EtatThermo, &Particule ) ;
  if ( strcmp( Donnees.methode, "DPDE_ito"     ) == 0 ) 
  	dynamique( idDPDE_ito, &Donnees, &EtatThermo, &Particule ) ;

/* fermeture des fichiers */
  fclose( Donnees.mdout ) ;
  fclose( Donnees.cntrl ) ;
  if ( strstr( Donnees.methode, "DPDE" ) != NULL ) fclose( Donnees.ftemp_int ) ;
  
  return erreur ;
}


