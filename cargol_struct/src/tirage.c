
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * tirage des nombres aleatoires a l'avance pour les tests DPD               * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"

double  **wxrand = NULL , **wyrand = NULL , **wzrand = NULL ;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

int tirage ( int Natom, long *idum ) {

  int erreur = 0 ;
  int iat, jat ;
  
  *idum = 1 ;

  for ( iat = 0 ; iat < Natom ; iat++ ) {
    for ( jat = 0 ; jat < Natom ; jat++ ) {
      wxrand[iat][jat] = gauss() ;
      wyrand[iat][jat] = gauss() ;
      wzrand[iat][jat] = gauss() ;
    }
  }

  return erreur;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

int tirage_old2 ( int Natom, long *idum ) {

  int erreur = 0 ;
  int iat, k ;

  for ( iat = 0 ; iat < Natom ; iat++ ) {
    for ( k = 0 ; k < NRAND ; k++ ) {
      wxrand[iat][k] = gasdev( idum ) ;
      wyrand[iat][k] = gasdev( idum ) ;
      wzrand[iat][k] = gasdev( idum ) ;
    }
  }

  return erreur;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

int tirage_old ( int Natom, long *idum ) {

  int  erreur = 0 ;
  int  iat, jat ;
  static double	 r1, r2, r3 ;

  for ( iat = 0 ; iat < Natom ; iat++ ) {
    for ( jat = 0 ; jat < Natom ; jat++ ) {

      if ( ran1( idum ) > 0.8 ) {
      // if ( jat % 2 == 0 ) {
      	r1 = gasdev( idum ) ;
      	r2 = gasdev( idum ) ;
      	r3 = gasdev( idum ) ;
      }

      wxrand[iat][jat] = r1 ;
      wyrand[iat][jat] = r2 ;
      wzrand[iat][jat] = r3 ;
    }
  }

  return erreur;
}

