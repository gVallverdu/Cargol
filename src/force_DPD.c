
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * forces DPD dissipative : force de friction, force aleatoire               * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

static int force_DPD_split( void ) ;
static int force_DPD_nosplit( void ) ;

int force_DPD( int idmethode, int idtest ) {

  int erreur = EXIT_SUCCESS ;

  if( idmethode != idtest ) return erreur ;

  if( idmethode == idDPDsplit ) {
    force_DPD_split() ;

  } else if( idmethode == idDPDnosplit ) {
    force_DPD_nosplit() ;

  } else {
    stop( "erreur : methode non reconnue dans force_DPD") ;

  }

  return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * cas d'un splitting complet NVE / DPD                                      * 
 * apres avoir calculer les forces DPD on modifie les vitesses               *
 * on ne modifie pas les tableaux frcx, frcy, frcz des forcesc conservatives *
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int force_DPD_split( void ) {

  int     erreur = EXIT_SUCCESS ;
  int     iat, jat, ivois, nvois ;
  int     ii, jj ;
  long    seed ;
  double  xij, yij, zij ;
  double  dist, dist2 ;
  double  sigma, racdt, amplitude ;
  double  chi, chi2 ;
  double  dvx, dvy, dvz ;
  double  fx, fy, fz ;
  double  wx, wy, wz ;
  double  masse_inv ;
  double  *fdpdx = NULL, *fdpdy = NULL, *fdpdz = NULL ;

  sigma = sqrt( 2. * xsi * boltzman * temp0 ) ; 	// sqrt( M^2 L^2 T^-3 )
  racdt = sqrt( dt ) ;
  amplitude = sigma / racdt ;				// sqrt (M^2 L^2 T^-4 ) = N
  
  seed = numerostep * nat * nbrevoisinmax + nat * (nat-1) / 2 ;
  
  // mise à zero des forces DPD
  fdpdx = calloc( nat, sizeof(double) ) ;
  fdpdy = calloc( nat, sizeof(double) ) ;
  fdpdz = calloc( nat, sizeof(double) ) ;
  for ( iat = 0 ; iat < nat ; iat++ ) {
    fdpdx[iat] = 0. ;
    fdpdy[iat] = 0. ;
    fdpdz[iat] = 0. ;
  }

  // boucle sur les paires d'atomes
  for ( iat = 0 ; iat < nat-1 ; iat++ ) {

    nvois = nbrevoisin[iat] ;
    for ( ivois = 0 ; ivois < nvois ; ivois++ ) {

      jat = listevoisin[iat][ivois] ;

      // vecteur iat -> jat
      xij = x[jat] - x[iat] ;
      yij = y[jat] - y[iat] ;
      zij = z[jat] - z[iat] ;

      // image de jat la plus proche de iat
      if ( xij >  boxx / 2. ) xij = xij - boxx ;
      if ( xij < -boxx / 2. ) xij = xij + boxx ;
      if ( yij >  boxy / 2. ) yij = yij - boxy ;
      if ( yij < -boxy / 2. ) yij = yij + boxy ;
      if ( zij >  boxz / 2. ) zij = zij - boxz ;
      if ( zij < -boxz / 2. ) zij = zij + boxz ;

      // distance iat - jat au carre
      dist2 = xij * xij + yij * yij + zij * zij ;

      if ( dist2 < rcut2 ) {

        // chi (sans unite) : fonction de cutt off simple = lineaire
        dist = sqrt(dist2) ;
        chi = 1. - dist / rcut ;
        chi2 = chi * chi ;

        // vitesse relative
        dvx = vx[iat] - vx[jat] ;
        dvy = vy[iat] - vy[jat] ;
        dvz = vz[iat] - vz[jat] ;

	// tirage
        wx = gasdev( &idum ) ;
        wy = gasdev( &idum ) ;
        wz = gasdev( &idum ) ;

        // DPD avec un pas complet sur les forces
        fx = - xsi * chi2 * dvx + amplitude * chi * wx ;
        fy = - xsi * chi2 * dvy + amplitude * chi * wy ;
        fz = - xsi * chi2 * dvz + amplitude * chi * wz ;

        fdpdx[iat] += fx ;
        fdpdy[iat] += fy ;
        fdpdz[iat] += fz ;

        fdpdx[jat] -= fx ;
        fdpdy[jat] -= fy ;
        fdpdz[jat] -= fz ;

      }
    }
  }
  
  // mise à jour des vitesses
  masse_inv = dt / masse ;
  
  for ( iat = 0 ; iat < nat ; iat++ ) {
    vx[iat] += masse_inv * fdpdx[iat] ;
    vy[iat] += masse_inv * fdpdy[iat] ;    
    vz[iat] += masse_inv * fdpdz[iat] ;
  }
  
  free( fdpdx ) ;
  free( fdpdy ) ;
  free( fdpdz ) ;
  
  return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * cas ou les forces DPD sont calculees au meme moment que les forces        *
 * conservatives                                                             * 
 * on inclut les forces DPD dans les forces conservatives : frcx, frcy, frcz *
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int force_DPD_nosplit( void ) {

  int     erreur = EXIT_SUCCESS ;
  int     iat, jat ;
  int     ivois, nvois ;
  double  xij, yij, zij ;
  double  dist, dist2 ;
  double  sigma, racdt, amplitude ;
  double  chi, chi2 ;
  double  dvx, dvy, dvz ;
  double  fdpdx, fdpdy, fdpdz ;
  double  wx, wy, wz ;

  sigma = sqrt( 2. * xsi * boltzman * temp0 ) ;		// sqrt( M^2 L^2 T^-3 )
  racdt = sqrt( dt ) ;
  amplitude = sigma / racdt ;				// M L T^-2 = force

  /* modification des forces pour la DPD */
  for ( iat = 0 ; iat < nat-1 ; iat++ ) {

    nvois = nbrevoisin[iat] ;
    for ( ivois = 0 ; ivois < nvois ; ivois++ ) {

      jat = listevoisin[iat][ivois] ;

      // vecteur iat -> jat
      xij = x[jat] - x[iat] ;
      yij = y[jat] - y[iat] ;
      zij = z[jat] - z[iat] ;

      // image de jat la plus proche de iat
      if ( xij >  boxx / 2. ) xij = xij - boxx ;
      if ( xij < -boxx / 2. ) xij = xij + boxx ;
      if ( yij >  boxy / 2. ) yij = yij - boxy ;
      if ( yij < -boxy / 2. ) yij = yij + boxy ;
      if ( zij >  boxz / 2. ) zij = zij - boxz ;
      if ( zij < -boxz / 2. ) zij = zij + boxz ;

      // distance iat - jat au carre
      dist2 = xij * xij + yij * yij + zij * zij ;

      if ( dist2 < rcut2 ) {

        // chi (sans unite) : fonction de cutt off simple = lineaire
        dist = sqrt(dist2) ;
        chi = 1. - dist / rcut ;
        chi2 = chi * chi ;

        // vitesse relative
        dvx = ( vx[iat] - vx[jat] ) ;
        dvy = ( vy[iat] - vy[jat] ) ;
        dvz = ( vz[iat] - vz[jat] ) ;

        // tirage
        wx = gasdev( &idum ) ;
        wy = gasdev( &idum ) ;
        wz = gasdev( &idum ) ;

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
      }
    }
  }

  return erreur ;
}

