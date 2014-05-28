
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * gestion des fichiers d'entre / sortie                                     * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"
#include "fichier.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * ouverture des fichiers                                                    * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 
int init_fichier( void ) {

  int erreur = EXIT_SUCCESS ;

  strcpy( nomfonction, "init_fichier() ") ;
  
  // noms des fichiers
  strcpy( amdout      , "mdout.dat" ) ;
  strcpy( amdcrd      , "mdcrd" ) ;
  strcpy( amdvel      , "mdvel" ) ;
  strcpy( acntrl      , "md_control" ) ;
  strcpy( aradial_dist, "radial_dist.dat" ) ;
  strcpy( arestart    , "restart" ) ;
  strcpy( atemp_int   , "temp_interne.dat" ) ;  

  // ouverture des fichiers de sorite recurent
  mdout = fopen( amdout, "w" ) ;
  if ( mdout == NULL ) erreur++ ;

  cntrl = fopen( acntrl, "w" ) ;
  if ( cntrl == NULL ) erreur++ ;

  return erreur ;

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * fermeture des fichiers                                                    * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int fermeture( void ) {

  int erreur = EXIT_SUCCESS ;
  
  strcpy( nomfonction, "fermeture()" ) ;

  fclose( mdout ) ;
  fclose( cntrl ) ;
  if ( strstr( methode, "DPDE" ) != NULL ) fclose( ftemp_int ) ;
  
  return erreur ;
}
