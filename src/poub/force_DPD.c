
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * force dissipative et force aleatoire pour la DPD                          * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

extern double gasdev( long * ) ;

int force_DPD( double dist2, int iat, int jat ) {

  int     erreur = 0 ;
  double  dist ;
  double  sigma, racdt, amplitude ;
  double  chi, chi2 ;
  double  dvx, dvy, dvz ;
  double  fdpdx, fdpdy, fdpdz ;
  double  wx, wy, wz ;

  double  r1, r2 ;
  int     k ;

  sigma = sqrt( 2. * xsi * boltzman * temp0 ) ;		// sqrt( M^2 L^2 T^-3 )
  racdt = sqrt( dt ) ;
  amplitude = sigma / racdt ;				// M L T^-2 = force

/* * * * * * * *  modification des forces pour la DPD  * * * * * * * * */

  // chi (sans unite) : fonction de cutt off simple = lineaire
  dist = sqrt(dist2) ;
  chi = 1. - dist / rcut ;
  chi2 = chi * chi ;

  // vitesse relative
  dvx = ( vx[iat] - vx[jat] ) ;
  dvy = ( vy[iat] - vy[jat] ) ;
  dvz = ( vz[iat] - vz[jat] ) ;

  // tirage
  /*
  wx = gasdev( &idum ) ;
  wy = gasdev( &idum ) ;
  wz = gasdev( &idum ) ;
  */
  
  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

  k = ( graine + iat + jat + 1 ) % NRAND ;
  r1 = wxrand[iat][k] ;
  k = ( graine + iat + jat + 1 ) % NRAND ;
  r2 = wxrand[jat][k] ;
  wx = ( r1 + r2 ) / sqrt(2.) ;

  k = ( graine + iat + jat + 2 ) % NRAND ;
  r1 = wyrand[iat][k] ;
  k = ( graine + iat + jat + 2 ) % NRAND ;
  r2 = wyrand[jat][k] ;
  wy = ( r1 + r2 ) / sqrt(2.) ;

  k = ( graine + iat + jat + 3 ) % NRAND ;
  r1 = wzrand[iat][k] ;
  k = ( graine + iat + jat + 3 ) % NRAND ;
  r2 = wzrand[jat][k] ;
  wz = ( r1 + r2 ) / sqrt(2.) ;

  /*
  wx = ( wxrand[iat][jat] + wxrand[jat][iat] ) / sqrt(2.) ;
  wy = ( wyrand[iat][jat] + wyrand[jat][iat] ) / sqrt(2.) ;
  wz = ( wzrand[iat][jat] + wzrand[jat][iat] ) / sqrt(2.) ;
  */

  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

  // ajout de la force de friction et de la force aleatoire
  fdpdx = - xsi * chi2 * dvx + amplitude * chi * wx ;
  fdpdy = - xsi * chi2 * dvy + amplitude * chi * wy ;
  fdpdz = - xsi * chi2 * dvz + amplitude * chi * wz ;

  // maj des forces
  frcx[iat] += fdpdx ;
  frcy[iat] += fdpdy ;
  frcz[iat] += fdpdz ;

  frcx[jat] -= fdpdx ;
  frcy[jat] -= fdpdy ;
  frcz[jat] -= fdpdz ;

  return erreur ;
}

