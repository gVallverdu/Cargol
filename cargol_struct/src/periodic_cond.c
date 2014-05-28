
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * application des conditions periodiaues aux limites                        * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"

int periodic_cond ( int Natom, Data *d, Particules *Particule[] ) {

  Particules *p = NULL ;

  int erreur = EXIT_SUCCESS ;
  int iat ;

  for ( iat = 0 ; iat < Natom ; iat++ ) {
  
    p = &((*Particule)[iat]) ;

    p->x = p->x - d->boxx * (double) ( floor( p->x / d->boxx ) ) ;
    p->y = p->y - d->boxy * (double) ( floor( p->y / d->boxy ) ) ;
    p->z = p->z - d->boxz * (double) ( floor( p->z / d->boxz ) ) ;

    // test
    if ( p->x > d->boxx || p->x < 0. ) erreur++ ;
    if ( p->y > d->boxy || p->y < 0. ) erreur++ ;
    if ( p->z > d->boxz || p->z < 0. ) erreur++ ;
    if ( erreur != 0 ) {
      printf("\n\niteration %d : ", d->numerostep);
      printf("atome %d en dehors de la boite\n", iat );
      printf("%10.4f "    , p->x * d->Ang ) ;
      printf("%10.4f "    , p->y * d->Ang ) ;
      printf("%10.4f \n\n", p->z * d->Ang ) ;
      stop("erreur dans periodic_cond.c") ;
    }
  }
	
  return erreur;
}

