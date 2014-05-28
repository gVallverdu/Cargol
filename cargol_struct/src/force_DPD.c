
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * DPD energie constante :                                                   * 
 * force dissipative, force aleatoire, energie et temperature interne        * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"

static int force_DPD_split( int, Data *, Particules ** ) ;
static int force_DPD_nosplit( int, Data *, Particules ** ) ;

int force_DPD( int idmethode, int idtest, int Natom, Data *Donnees, Particules *Particule[] ) {

  int erreur = EXIT_SUCCESS ;

  if( idmethode != idtest ) return erreur ;

  if( idmethode == idDPDsplit ) {
    force_DPD_split( Natom, Donnees, Particule ) ;

  } else if( idmethode == idDPDnosplit ) {
    force_DPD_nosplit( Natom, Donnees, Particule ) ;

  } else {
    stop("erreur : methode non reconnue dans force_DPD" ) ;

  }

  return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * cas d'un splitting NVE / DPD                                              * 
 * apres avoir calculer les forces DPD on modifie les vitesses               *
 * on ne modifie pas les tableaux frcx, frcy, frcz des forcesc conservatives *
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int force_DPD_split( int Natom, Data *d, Particules *Particule[] ) {

  Particules *pi = NULL, *pj = NULL ;

  int     erreur = EXIT_SUCCESS ;
  int     iat, jat, ivois, nvois ;
  double  xij, yij, zij ;
  double  dist, dist2 ;
  double  dt, sigma, racdt, amplitude ;
  double  chi, chi2 ;
  double  dvx, dvy, dvz ;
  double  fx, fy, fz ;
  double  wx, wy, wz ;
  double  masse_inv ;
  double  *fdpdx = NULL, *fdpdy = NULL, *fdpdz = NULL ;

  dt = d->dt ;
  sigma = sqrt( 2. * d->xsi * d->boltzman * d->temp0 ) ; 	// sqrt( M^2 L^2 T^-3 )
  racdt = sqrt( dt ) ;
  amplitude = sigma / racdt ;					// sqrt (M^2 L^2 T^-4 ) = N
  
  // mise à zero des forces DPD
  fdpdx = calloc( Natom, sizeof(double) ) ;
  fdpdy = calloc( Natom, sizeof(double) ) ;
  fdpdz = calloc( Natom, sizeof(double) ) ;
  for ( iat = 0 ; iat < Natom ; iat++ ) {
    fdpdx[iat] = 0. ;
    fdpdy[iat] = 0. ;
    fdpdz[iat] = 0. ;
  }

  // boucle sur les paires d'atomes
  for ( iat = 0 ; iat < Natom-1 ; iat++ ) {
    pi = &((*Particule)[iat]) ;

    nvois = pi->nbrevoisin ;
    for ( ivois = 0 ; ivois < nvois ; ivois++ ) {

      jat = pi->listevoisin[ivois] ;
      pj = &((*Particule)[jat]) ;

      // vecteur iat -> jat
      xij = pj->x - pi->x ;
      yij = pj->y - pi->y ;
      zij = pj->z - pi->z ;

      // image de jat la plus proche de iat
      if ( xij >  d->boxx / 2. ) xij = xij - d->boxx ;
      if ( xij < -d->boxx / 2. ) xij = xij + d->boxx ;
      if ( yij >  d->boxy / 2. ) yij = yij - d->boxy ;
      if ( yij < -d->boxy / 2. ) yij = yij + d->boxy ;
      if ( zij >  d->boxz / 2. ) zij = zij - d->boxz ;
      if ( zij < -d->boxz / 2. ) zij = zij + d->boxz ;

      // distance iat - jat au carre
      dist2 = xij * xij + yij * yij + zij * zij ;

      if ( dist2 < d->rcut2 ) {

        // chi (sans unite) : fonction de cutt off simple = lineaire
        dist = sqrt(dist2) ;
        chi = 1. - dist / d->rcut ;
        chi2 = chi * chi ;

        // vitesse relative
        dvx = pi->vx - pj->vx ;
        dvy = pi->vy - pj->vy ;
        dvz = pi->vz - pj->vz ;

        wx = gasdev( &(d->idum) ) ;
        wy = gasdev( &(d->idum) ) ;
        wz = gasdev( &(d->idum) ) ;

        // DPD avec un pas complet sur les forces
        fx = - d->xsi * chi2 * dvx + amplitude * chi * wx ;
        fy = - d->xsi * chi2 * dvy + amplitude * chi * wy ;
        fz = - d->xsi * chi2 * dvz + amplitude * chi * wz ;

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
  masse_inv = dt / d->masse ;
  
  for ( iat = 0 ; iat < Natom ; iat++ ) {
    pi = &((*Particule)[iat]) ;
    
    pi->vx += masse_inv * fdpdx[iat] ;
    pi->vy += masse_inv * fdpdy[iat] ;    
    pi->vz += masse_inv * fdpdz[iat] ;
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

static int force_DPD_nosplit( int Natom, Data *d, Particules *Particule[] ) {

  Particules *pi = NULL, *pj = NULL ;

  int     erreur = EXIT_SUCCESS ;
  int     iat, jat, ivois, nvois ;
  double  xij, yij, zij ;
  double  dist, dist2 ;
  double  sigma, racdt, amplitude ;
  double  chi, chi2 ;
  double  dvx, dvy, dvz ;
  double  fdpdx, fdpdy, fdpdz ;
  double  wx, wy, wz ;

  sigma = sqrt( 2. * d->xsi * d->boltzman * d->temp0 ) ;	// sqrt( M^2 L^2 T^-3 )
  racdt = sqrt( d->dt ) ;
  amplitude = sigma / racdt ;					// M L T^-2 = force

  // boucle sur les paires d'atomes
  for ( iat = 0 ; iat < Natom-1 ; iat++ ) {
    pi = &((*Particule)[iat]) ;

    nvois = pi->nbrevoisin ;
    for ( ivois = 0 ; ivois < nvois ; ivois++ ) {

      jat = pi->listevoisin[ivois] ;
      pj = &((*Particule)[jat]) ;

      // vecteur iat -> jat
      xij = pj->x - pi->x ;
      yij = pj->y - pi->y ;
      zij = pj->z - pi->z ;

      // image de jat la plus proche de iat
      if ( xij >  d->boxx / 2. ) xij = xij - d->boxx ;
      if ( xij < -d->boxx / 2. ) xij = xij + d->boxx ;
      if ( yij >  d->boxy / 2. ) yij = yij - d->boxy ;
      if ( yij < -d->boxy / 2. ) yij = yij + d->boxy ;
      if ( zij >  d->boxz / 2. ) zij = zij - d->boxz ;
      if ( zij < -d->boxz / 2. ) zij = zij + d->boxz ;

      // distance iat - jat au carre
      dist2 = xij * xij + yij * yij + zij * zij ;

      if ( dist2 < d->rcut2 ) {

        // chi (sans unite) : fonction de cutt off simple = lineaire
        dist = sqrt(dist2) ;
        chi = 1. - dist / d->rcut ;
        chi2 = chi * chi ;

        // vitesse relative
        dvx = ( pi->vx - pj->vx ) ;
        dvy = ( pi->vy - pj->vy ) ;
        dvz = ( pi->vz - pj->vz ) ;

        // tirage
        wx = gasdev( &(d->idum) ) ;
        wy = gasdev( &(d->idum) ) ;
        wz = gasdev( &(d->idum) ) ;

        // ajout de la force de friction et de la force aleatoire
        fdpdx = - d->xsi * chi2 * dvx + amplitude * chi * wx ;
        fdpdy = - d->xsi * chi2 * dvy + amplitude * chi * wy ;
        fdpdz = - d->xsi * chi2 * dvz + amplitude * chi * wz ;

        // maj des forces
        pi->frcx += fdpdx ;
        pi->frcy += fdpdy ;
        pi->frcz += fdpdz ;

        pj->frcx -= fdpdx ;
        pj->frcy -= fdpdy ;
        pj->frcz -= fdpdz ;
      }
    }
  }

  return erreur ;
}

