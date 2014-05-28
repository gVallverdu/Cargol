
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * construction de la liste des voisins symetrisee                           * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"

int voisin( int Natom, Data *d, Particules *Particule[] ) {

  Particules *pi = NULL, *pj = NULL ;

  int     erreur = EXIT_SUCCESS ;
  int     iat, jat ;
  double  dist2 ;
  double  xij, yij, zij ;

  d->nbremajvoisin++ ;

  // boucle sur tous les atomes
  for ( iat = 0 ; iat < Natom-1 ; iat++ ) {
    // particule jat 
    pi = &((*Particule)[iat]) ;

    pi->nbrevoisin = 0 ;

    // re-initialisation des deplacement des atomes
    pi->dx = 0. ;
    pi->dy = 0. ;
    pi->dz = 0. ;

    // boucle sur les tous les atomes
    for ( jat = iat+1 ; jat < Natom ; jat++ ) {
 
      // particule jat 
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

      // distance iat - jat
      dist2 = xij * xij + yij * yij + zij * zij ;

      if ( dist2 < d->rverlet2 ) {
        // on a trouve un voisin
        pi->listevoisin[ pi->nbrevoisin ] = jat ;
        pi->nbrevoisin++ ;
			
       	if( pi->nbrevoisin == d->nbrevoisinmax + 1 ) {
       	  stop("depassement du nombre de voisin : changer le parametre nbrevoisinmax dans l'input");
        }
      }
    }
  }

  // pour le dernier atome
  pi = &((*Particule)[Natom-1]) ;
  pi->dx = 0. ;
  pi->dy = 0. ;
  pi->dz = 0. ;

  return erreur;
}

