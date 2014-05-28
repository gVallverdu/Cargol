
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * tirage des nombres aleatoires a l'avance pour les tests DPD parallele     * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

extern double gasdev( long * ) ;
extern double ran1( long * ) ;
extern double gauss( void ) ;


int tirage ( void ) {

  int erreur = 0 ;
  int iat, jat ;

  for ( iat = 0 ; iat < nat ; iat++ ) {
    for ( jat = 0 ; jat < nat ; jat++ ) {
      wxrand[iat][jat] = gauss() ;
      wyrand[iat][jat] = gauss() ;
      wzrand[iat][jat] = gauss() ;
    }
  }

  return erreur;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

int tirage_old2 ( void ) {

  int erreur = 0 ;
  int iat, k ;

  for ( iat = 0 ; iat < nat ; iat++ ) {
    for ( k = 0 ; k < NRAND ; k++ ) {
      wxrand[iat][k] = gasdev( &idum ) ;
      wyrand[iat][k] = gasdev( &idum ) ;
      wzrand[iat][k] = gasdev( &idum ) ;
    }
  }

  return erreur;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

int tirage_old ( void ) {

  int  erreur = 0 ;
  int  iat, jat ;
  static double	 r1, r2, r3 ;

  for ( iat = 0 ; iat < nat ; iat++ ) {
    for ( jat = 0 ; jat < nat ; jat++ ) {

      if ( ran1( &idum ) > 0.8 ) {
      // if ( jat % 2 == 0 ) {
      	r1 = gasdev( &idum ) ;
      	r2 = gasdev( &idum ) ;
      	r3 = gasdev( &idum ) ;
      }

      wxrand[iat][jat] = r1 ;
      wyrand[iat][jat] = r2 ;
      wzrand[iat][jat] = r3 ;
    }
  }

  return erreur;
}

