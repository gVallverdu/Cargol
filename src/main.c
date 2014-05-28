
/* librairies, define et structure */
#include "main.h"

/* declarations des variables globales */
#include "globales.h"

extern int commande( int , char ** ) ;
extern int message1( void ) ;
extern int init_calcul( void ) ;
extern int dynamique( int ) ;

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
  *                                                                           * 
  * programme princpal                                                        * 
  *                                                                           * 
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int main( int narg, char **arg ) {

  int erreur = EXIT_SUCCESS ;

  start = clock() ;

/* message d'accueil */
  message1() ;

/* gestion de la ligne de commande */
  commande( narg, arg ) ;

/* Lecture de l'input et initialisation des variables : */
  init_calcul() ;

/* dynamique NVE */
  if ( strcmp( methode, "NVE"          ) == 0 ) dynamique( idNVE ) ; 

/* dynamique de langevin */
  if ( strcmp( methode, "langevin"     ) == 0 ) dynamique( idlangevin ) ; 
  if ( strcmp( methode, "LNVT"         ) == 0 ) dynamique( idLNVT ) ; 

/* dynamique DPD */
  if ( strcmp( methode, "DPD"          ) == 0 ) dynamique( idDPDsplit ) ;
  if ( strcmp( methode, "DPDnosplit"   ) == 0 ) dynamique( idDPDnosplit ) ;

/* dynamique DPDE */
  if ( strcmp( methode, "DPDE"         ) == 0 ) dynamique( idDPDE ) ; 
  if ( strcmp( methode, "DPDE_alamain" ) == 0 ) dynamique( idDPDE_alamain ) ;
  if ( strcmp( methode, "DPDE_ito"     ) == 0 ) dynamique( idDPDE_ito ) ;  

/* fermeture des fichiers */
  fclose( mdout ) ;
  fclose( cntrl ) ;
  if ( strstr( methode, "DPDE" ) != NULL ) fclose( ftemp_int ) ;
  
  return erreur;
}


