
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * liberation de la memoire de tous les tableaux                             * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"

int free_memoire ( Data *Donnees, Particules *Particule[] ) {

  int erreur = EXIT_SUCCESS ;

  // structures particule
  free( *Particule ) ;

  // distribution radiale
  free ( Donnees->radial_dist ) ;

  return erreur ;
}

