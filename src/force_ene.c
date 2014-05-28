
 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  *                                                                           *
  * calcul des forces et de l'energie potentielle totale                      *
  *                                                                           *
  * le calcul prend en compte les conditions periodiques aux limites          *
  * on cherche l'image de jat la plus proche de iat                           *
  * on utilise le principe des listes de verlet                               *
  * le calcul des forces est symetrise car les listes sont symetrisees        *
  *                                                                           *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

/* fonctions locales */
static int force_ene_LJ( void ) ;
static int force_ene_exp6( void ) ;
static int force_ene_morse( void ) ;

int force_ene( void ) {

  int erreur = EXIT_SUCCESS ;
  int iat ;

  // initialisation energie potentielle
  eptot = 0. ;

  // initialisation des forces 
  for ( iat = 0 ; iat < nat ; iat++ ) {
    frcx[iat] = 0. ;
    frcy[iat] = 0. ;
    frcz[iat] = 0. ;
  }

  // initialisation du viriel
  virielx = viriely = virielz = 0. ;

  // calcul des forces et de l'energie
  if( potentiel == LenardJones ) {
    erreur += force_ene_LJ() ;

  } else if( potentiel == Exp6 ) {
    erreur += force_ene_exp6() ;

  } else if( potentiel == Morse ) {
    erreur += force_ene_morse() ;

  } else {
    stop("erreur potentiel") ;

  }

  return erreur;
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  *                                                                           *
  * Potentiel Lenard jones                                                    *
  *                                                                           *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int force_ene_LJ( void ) {

  int     erreur = EXIT_SUCCESS ;
  int     iat, jat, ivois, k, nvois ;
  double  xij, yij, zij ;
  double  dist2 ;
  double  dist_inv2, dist_inv6 ;
  double  coef, df, ene ;
  double  fx, fy, fz ;

  // boucles sur toutes les paires d'atomes
  for ( iat = 0 ; iat < nat-1 ; iat++ ) {

    nvois = nbrevoisin[iat] ;
    for ( ivois = 0 ; ivois < nvois ; ivois++ ) {

      jat = listevoisin [iat] [ivois] ;

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

        dist_inv2 = 1. / dist2 ;
        dist_inv6 = dist_inv2 * dist_inv2 * dist_inv2 ;

        // energie
        coef = LJ4eps * dist_inv6 * LJsigma6 ;

        ene = coef * ( LJsigma6 * dist_inv6 - 1.0 ) - Ep_shift ;

        eptot += ene ;
				
        // forces sur iat et jat, on calcule les forces = - grad E
        // derivee par rapport a dist puis aux coordonnees xyz

        df = coef * ( 6. - 12. * LJsigma6 * dist_inv6 ) ;
        df = df * dist_inv2 ;

        fx = df * xij ;
        fy = df * yij ;
        fz = df * zij ;

        frcx[iat] += fx ;
        frcy[iat] += fy ;
        frcz[iat] += fz ;

        frcx[jat] -= fx ;
        frcy[jat] -= fy ;
        frcz[jat] -= fz ;

        // calcul du viriel => Allen Tildesley p48
        // attention au signe du vecteur ij
        virielx = virielx - xij * fx ;
        viriely = viriely - yij * fy ;
        virielz = virielz - zij * fz ;

        // mise a jour de g(r)
        k = (int) ( sqrt(dist2) / pas_radial_dist ) ;
        radial_dist[k] += 2. ; // contribution iat + jat = 2

      } 
    } 
  } 

  return erreur;
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  *                                                                           *
  * Potentiel exponnentielle 6                                                *
  *                                                                           *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int force_ene_exp6( void ) {

  int     erreur = EXIT_SUCCESS ;
  int     iat, jat, ivois, k, nvois ;
  double  xij, yij, zij ;
  double  dist, dist2 ;
  double  dist_inv2, dist_inv6 ;
  double  Aexp, Cr6, df, ene ;
  double  fx, fy, fz ;

  // boucles sur toutes les paires d'atomes
  for ( iat = 0 ; iat < nat-1 ; iat++ ) {

    nvois = nbrevoisin[iat] ;
    for ( ivois = 0 ; ivois < nvois ; ivois++ ) {

      jat = listevoisin [iat] [ivois] ;

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

        dist = sqrt( dist2 ) ;
        dist_inv2 = 1. / dist2 ;
        dist_inv6 = dist_inv2 * dist_inv2 * dist_inv2 ;

        // energie
	Aexp = Exp6A * exp( - Exp6B * dist ) ;
        Cr6 = Exp6C * dist_inv6 ;

        ene = Aexp - Cr6 - Ep_shift ;

        eptot += ene ;
				
        // forces sur iat et jat, on calcule les forces = - grad E
        // derivee par rapport a dist puis aux coordonnees xyz

        df = - Exp6B * Aexp + 6. * Cr6 / dist ;
        df = df / dist ;

        fx = df * xij ;
        fy = df * yij ;
        fz = df * zij ;

        frcx[iat] += fx ;
        frcy[iat] += fy ;
        frcz[iat] += fz ;

        frcx[jat] -= fx ;
        frcy[jat] -= fy ;
        frcz[jat] -= fz ;

        // calcul du viriel => Allen Tildesley p48
        // attention au signe du vecteur ij
        virielx = virielx - xij * fx ;
        viriely = viriely - yij * fy ;
        virielz = virielz - zij * fz ;

        // mise a jour de g(r)
        k = (int) ( sqrt(dist2) / pas_radial_dist ) ;
        radial_dist[k] += 2. ; // contribution iat + jat = 2

      } 
    } 
  } 

  return erreur;
}


 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  *                                                                           *
  * Potentiel de morse                                                        *
  *                                                                           *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int force_ene_morse( void ) {

  int     erreur = EXIT_SUCCESS ;
  int     iat, jat, ivois, k, nvois ;
  double  xij, yij, zij ;
  double  dist, dist2 ;
  double  coefexp, df, ene ;
  double  fx, fy, fz ;

  // boucles sur toutes les paires d'atomes
  for ( iat = 0 ; iat < nat-1 ; iat++ ) {

    nvois = nbrevoisin[iat] ;
    for ( ivois = 0 ; ivois < nvois ; ivois++ ) {

      jat = listevoisin [iat] [ivois] ;

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

        dist = sqrt( dist2 ) ;

        // energie
        coefexp = exp( - alpha * ( dist - R_eq ) ) ;
        ene = Dmorse * ( 1. - coefexp) * ( 1. - coefexp ) - Ep_shift ;
        eptot += ene ;
				
        // forces sur iat et jat, on calcule les forces = - grad E
        df = 2. * Dmorse * alpha * coefexp * ( 1. - coefexp ) ;
        df = df / dist ;

        fx = df * xij ;
        fy = df * yij ;
        fz = df * zij ;

        frcx[iat] += fx ;
        frcy[iat] += fy ;
        frcz[iat] += fz ;

        frcx[jat] -= fx ;
        frcy[jat] -= fy ;
        frcz[jat] -= fz ;

        // calcul du viriel => Allen Tildesley p48
        // attention au signe du vecteur ij
        virielx = virielx - xij * fx ;
        viriely = viriely - yij * fy ;
        virielz = virielz - zij * fz ;

        // mise a jour de g(r)
        k = (int) ( sqrt(dist2) / pas_radial_dist ) ;
        radial_dist[k] += 2. ; // contribution iat + jat = 2

      } 
    } 
  } 

  return erreur;
}

