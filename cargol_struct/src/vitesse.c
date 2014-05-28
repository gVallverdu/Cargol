
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * second 1/2 pas vitesse                                                    * 
 *                                                                           * 
 * algorithme de verlet vitesse                                              * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"

static int vitesseNVE( int , Data * , Particules ** ) ;
static int vitesse_langevin( int , Data * , Particules ** );
static int vitesseLNVT( int , Data * , Particules ** );

int vitesse( int idmethode, int Natom, Data *Donnees, Particules *Particule[] ) {

  int erreur = EXIT_SUCCESS ;

  if ( idmethode == idlangevin ) {
    vitesse_langevin( Natom, Donnees, Particule ) ;

  } else if ( idmethode == idLNVT ) {
    vitesseLNVT( Natom, Donnees, Particule ) ;
   
  } else {
    vitesseNVE( Natom, Donnees, Particule ) ;

  } 

  return erreur;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * second 1/2 pas vitesse dans le cas NVE                                    * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int vitesseNVE( int Natom, Data *d , Particules *Particule[] ) {

  Particules *p = NULL ;
  
  int    erreur = EXIT_SUCCESS ;
  int    iat ;
  double mass_inv ;

  mass_inv = 0.5 * d->dt / d->masse ;

  for ( iat = 0 ; iat < Natom ; iat++ ) {
    p = &((*Particule)[iat]) ;
				
    /* Vitesse a t + dt */		
    p->vx += mass_inv * p->frcx ;
    p->vy += mass_inv * p->frcy ;
    p->vz += mass_inv * p->frcz ;
  }
	
  return erreur;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * second 1/2 pas vitesse incluant la friction et le deplacement aleatoire   * 
 * implementation de langevin stamp3 :  BBK                                  * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int vitesseLNVT( int Natom, Data *d , Particules *Particule[] ) {

  Particules *p = NULL ;
  
  int    erreur = EXIT_SUCCESS ;
  int    iat ;
  double mass_inv, sigma, gamma, dt ;

  dt = d-> dt ;
  mass_inv = 0.5 * dt / d->masse ;

  gamma = 0.5 * d->xsi * dt ;
  sigma = sqrt( ( 1. + gamma ) * 2. * d->xsi * d->boltzman * d->temp0 / dt ) ;

  for ( iat = 0 ; iat < Natom ; iat++ ) {
    p = &((*Particule)[iat]) ;
			
    /* rajout de la force aleatoire */
    p->frcx += sigma * sqrt(d->masse) * gasdev( &(d->idum) ) ;
    p->frcy += sigma * sqrt(d->masse) * gasdev( &(d->idum) ) ;
    p->frcz += sigma * sqrt(d->masse) * gasdev( &(d->idum) ) ;

    /* dernier demi pas vitesse */
    p->vx = ( p->vx + mass_inv * p->frcx ) / ( 1. + 0.5 * dt * d->xsi ) ;
    p->vy = ( p->vy + mass_inv * p->frcy ) / ( 1. + 0.5 * dt * d->xsi ) ;
    p->vz = ( p->vz + mass_inv * p->frcz ) / ( 1. + 0.5 * dt * d->xsi ) ;

  }
	
  return erreur;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * second 1/2 pas vitesse incluant la friction et le deplacement aleatoire   * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int vitesse_langevin( int Natom, Data *d, Particules *Particule[] ) {

  Particules *p = NULL ;
  
  int    erreur = EXIT_SUCCESS ;
  int    iat ;
  double mass_inv, alpha, sigma, dt ;
  
  dt = d->dt ;
  mass_inv = 0.5 * dt / d->masse ;

  alpha = exp( - d->xsi * dt / d->masse ) ;
  sigma = sqrt( ( 1. - alpha * alpha ) * d->boltzman * d->temp0 / d->masse ) ;

  for ( iat = 0 ; iat < Natom ; iat++ ) {
    p = &((*Particule)[iat]) ;
				
    /* dernier demi pas vitesse : partie NVE */
    p->vx += mass_inv * p->frcx ;
    p->vy += mass_inv * p->frcy ;
    p->vz += mass_inv * p->frcz ;

    /* ajout de la friction et de la force aleatoire */
    p->vx = alpha * p->vx + sigma * gasdev( &(d->idum) ) ;
    p->vy = alpha * p->vy + sigma * gasdev( &(d->idum) ) ;
    p->vz = alpha * p->vz + sigma * gasdev( &(d->idum) ) ;

  }
	
  return erreur;
}
