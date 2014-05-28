
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * arret du programme avec commentaire                                       * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 
#include <stdio.h>
#include <stdlib.h>

extern int  numerostep ;

void stop( char *message ) {

  printf("\n\narret a l'iteration %d \n", numerostep ) ;
  printf("\n \t %s \n\n", message ) ;

  exit(EXIT_SUCCESS) ;

}
