
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * arret du programme avec commentaire                                       * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 
#include <stdio.h>
#include <stdlib.h>

void stop( char *message ) {

  printf("\n \t %s \n\n", message ) ;

  exit(EXIT_SUCCESS) ;

}
