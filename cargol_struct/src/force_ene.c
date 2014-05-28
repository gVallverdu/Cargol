
 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  *                                                                           *
  * calcul des forces et de l'energie potentielle totale                      *
  *                                                                           *
  * cas d'un potentiel Lennard Jones                                          *
  * le calcul prend en compte les conditions periodiques aux limites          *
  * on cherche l'image de jat la plus proche de iat                           *
  * on utilise le principe des listes de verlet                               *
  * le calcul des forces est symetrise car les listes sont symetrisees        *
  *                                                                           *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"

int force_ene( int idmethode, Data *d, Etat *e, Particules *Particule[] ) {

  Particules *pi = NULL, *pj = NULL ;

  int     erreur = EXIT_SUCCESS ;
  int     iat, jat, ivois, k, nvois ;
  double  xij, yij, zij ;
  double  dist2 ;
  double  dist_inv2, dist_inv6 ;
  double  coef, df, ene ;
  double  fx, fy, fz ;

  // initialisation energie potentielle
  e->eptot = 0. ;

  // initialisation des forces 
  for ( iat = 0 ; iat < e->nat ; iat++ ) {
    pi = &((*Particule)[iat]) ;
  
    pi->frcx = 0. ;
    pi->frcy = 0. ;
    pi->frcz = 0. ;
  }

  // initialisation du viriel
  e->virielx = e->viriely = e->virielz = 0. ;
	
  // boucles sur toutes les paires d'atomes
  for ( iat = 0 ; iat < e->nat-1 ; iat++ ) {

    /* on recupere la structure de la particule iat */
    pi = &((*Particule)[iat]) ;
    
    nvois = pi->nbrevoisin ;
    for ( ivois = 0 ; ivois < nvois ; ivois++ ) {

      /* on recupere la structure de la particule jat */
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

        dist_inv2 = 1. / dist2 ;
        dist_inv6 = dist_inv2 * dist_inv2 * dist_inv2 ;

        // energie
        coef = d->LJ4eps * dist_inv6 * d->LJsigma6 ;

        ene = coef * ( d->LJsigma6 * dist_inv6 - 1.0 ) - d->LJshift ;

        e->eptot += ene ;
				
        // forces sur iat et jat, on calcule les forces = - grad E
        // derivee par rapport a dist puis aux coordonnees xyz

        df = coef * ( 6. - 12. * d->LJsigma6 * dist_inv6 ) ;
        df = df * dist_inv2 ;

        fx = df * xij ;
        fy = df * yij ;
        fz = df * zij ;

        pi->frcx += fx ;
        pi->frcy += fy ;
        pi->frcz += fz ;

        pj->frcx -= fx ;
        pj->frcy -= fy ;
        pj->frcz -= fz ;

        // calcul du viriel => Allen Tildesley p48
        // attention au signe du vecteur ij
        e->virielx -= xij * fx ;
        e->viriely -= yij * fy ;
        e->virielz -= zij * fz ;

        // mise a jour de g(r)
        k = (int) ( sqrt(dist2) / d->pas_radial_dist ) ;
        d->radial_dist[k] += 2. ; // contribution iat + jat = 2

      }  // if dist2 < rcut2
    }  // ivois
  }  // iat

  return erreur;
}

