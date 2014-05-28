
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * mise a jour des positions et 1/2 pas vitesse                              * 
 *                                                                           * 
 * algorithme de verlet vitesse                                              * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"

static int positionNVE( int , Data * , Particules ** ) ;
static int positionLNVT( int , Data * , Particules ** ) ;

int position( int idmethode, int Natom, Data *Donnees, Particules *Particule[] ) {

  int erreur = EXIT_SUCCESS ;

  if ( idmethode == idLNVT ) {
    positionLNVT( Natom, Donnees, Particule ) ;
    
  } else {
    positionNVE( Natom, Donnees, Particule ) ;

  }

  return erreur;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * mise a jour des positions et 1/2 pas vitesse dans le cas NVE              * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int positionNVE( int Natom, Data *d, Particules *Particule[] ) {

  Particules *p = NULL ;

  int	 erreur = EXIT_SUCCESS ;
  int	 iat ;
  double mass_inv ;
  double x2, y2, z2 ;
  double deplacement ;

  mass_inv = 0.5 * d->dt / d->masse ;
  d->majvoisin = 0 ;

  for ( iat = 0 ; iat < Natom ; iat++ ) {
    p = &((*Particule)[iat]) ;
	
    // vitesse a t + 1/2 dt
    p->vx += mass_inv * p->frcx ;
    p->vy += mass_inv * p->frcy ;
    p->vz += mass_inv * p->frcz ;

    // position a t + dt utilisant les vitesses a t+1/2
    x2 = p->x + p->vx * d->dt ;
    y2 = p->y + p->vy * d->dt ;
    z2 = p->z + p->vz * d->dt ;

    // test rafraichissement de la table de voisins
    p->dx += fabs( x2 - p->x ) ;
    p->dy += fabs( y2 - p->y ) ;
    p->dz += fabs( z2 - p->z ) ;

    deplacement = p->dx * p->dx + p->dy * p->dy + p->dz * p->dz ;

    if ( deplacement > d->dmax2 ) d->majvoisin++ ;

    // maj des positions
    p->x = x2 ;
    p->y = y2 ;
    p->z = z2 ;
  }

  return erreur;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * mise a jour des positions et 1/2 pas vitesse dans le cas LNVT BBK         * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int positionLNVT( int Natom, Data *d, Particules *Particule[] ) {

  Particules *p = NULL ;

  int	  erreur = EXIT_SUCCESS ;
  int	  iat ;
  double  mass_inv, mass_inv2, dtxsi, dt ;
  double  x2, y2, z2 ;
  double  deplacement ;

  dt = d->dt ;
  mass_inv  = 0.5 * dt / d->masse ;
  mass_inv2 = 0.5 * dt * dt / d->masse ;
  dtxsi     = 0.5 * d->xsi * dt ;

  d->majvoisin = 0 ;

  for ( iat = 0 ; iat < Natom ; iat++ ) {
    p = &((*Particule)[iat]) ;
	
    // position a t + dt (pas NVE)
    x2 = p->x + p->vx * dt + mass_inv2 * p->frcx ;
    y2 = p->y + p->vy * dt + mass_inv2 * p->frcy ;
    z2 = p->z + p->vz * dt + mass_inv2 * p->frcz ;

    // force de friction
    x2 -= dt * dtxsi * p->vx ;
    y2 -= dt * dtxsi * p->vy ;
    z2 -= dt * dtxsi * p->vz ;

    // vitesse a t + 1/2 dt
    p->vx += mass_inv * p->frcx - dtxsi * p->vx ;
    p->vy += mass_inv * p->frcy - dtxsi * p->vy ;
    p->vz += mass_inv * p->frcz - dtxsi * p->vz ;

    // test rafraichissement de la table de voisins
    p->dx += fabs( x2 - p->x ) ;
    p->dy += fabs( y2 - p->y ) ;
    p->dz += fabs( z2 - p->z ) ;

    deplacement = p->dx * p->dx + p->dy * p->dy + p->dz * p->dz ;

    if ( deplacement > d->dmax2 ) d->majvoisin++ ;

    // maj des positions
    p->x = x2 ;
    p->y = y2 ;
    p->z = z2 ;
  }

  return erreur;
}


