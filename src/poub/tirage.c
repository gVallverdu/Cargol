
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * tirage des nombres aleatoires a l'avance pour les tests DPD parallele     * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

extern double gasdev( long * ) ;
extern double ran1( long * ) ;

int tirage ( void ) {

  int            erreur = 0 ;
  int            iat, jat ; // ivois ;
  static double	 r1, r2, r3 ;

  for ( iat = 0 ; iat < nat ; iat++ ) {
    for ( jat = 0 ; jat < nat ; jat++ ) {

      if ( random_number() > 0.8 ) {
      	r1 = gaussienne() ;
      	r2 = gaussienne() ;
      	r3 = gaussienne() ;
      }

      wxrand[iat][jat] = r1 ;
      wyrand[iat][jat] = r2 ;
      wzrand[iat][jat] = r3 ;
    }
  }

  return erreur;
}

/*
  for ( iat = 0 ; iat < nat ; iat++ ) {
    for ( ivois = 0 ; ivois < nbrevoisinmax ; ivois++ ) {
      wxrand[iat][ivois] = gasdev( &idum ) ;
      wyrand[iat][ivois] = gasdev( &idum ) ;
      wzrand[iat][ivois] = gasdev( &idum ) ;
    }
  }
*/
