
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * second 1/2 pas vitesse                                                    * 
 *                                                                           * 
 * algorithme de verlet vitesse                                              * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

static int vitesseNVE( void );
static int vitesse_langevin( void );
static int vitesseLNVT( void );

int vitesse( int idmethode ) {

  int erreur = EXIT_SUCCESS ;

  if ( idmethode == idNVE ) {
    vitesseNVE() ;

  } else if ( idmethode == idlangevin ) {
    vitesse_langevin() ;

  } else if ( idmethode == idLNVT ) {
    vitesseLNVT() ;

  } else if ( idmethode == idDPDnosplit ) {
    vitesseNVE() ;
    
  } else if ( idmethode == idDPDsplit ) {
    vitesseNVE() ;
    
  } else if ( idmethode == idDPDE || idmethode == idDPDE_alamain || idmethode == idDPDE_ito ) {
    vitesseNVE() ;

  } 

  return erreur;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * second 1/2 pas vitesse dans le cas NVE                                    * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int vitesseNVE( void ) {

  int    erreur = EXIT_SUCCESS ;
  int    iat ;
  double mass_inv ;

  mass_inv = 0.5 * dt / masse ;

  for ( iat = 0 ; iat < nat ; iat++ ) {
				
    /* Vitesse a t + dt */		
    vx[iat] += mass_inv * frcx[iat] ;
    vy[iat] += mass_inv * frcy[iat] ;
    vz[iat] += mass_inv * frcz[iat] ;

  }
	
  return erreur;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * second 1/2 pas vitesse incluant la friction et le deplacement aleatoire   * 
 * implementation de langevin stamp3 :  BBK                                  * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int vitesseLNVT( void ) {

  int    erreur = EXIT_SUCCESS ;
  int    iat ;
  double mass_inv, sigma, gamma ;

  mass_inv = 0.5 * dt / masse ;

  gamma = 0.5 * xsi * dt ;
  sigma = sqrt( ( 1. + gamma ) * 2. * xsi * boltzman * temp0 / dt ) ;

  for ( iat = 0 ; iat < nat ; iat++ ) {
				
    /* rajout de la force aleatoire */
    frcx[iat] += sigma * sqrt(masse) * gasdev( &idum ) ;
    frcy[iat] += sigma * sqrt(masse) * gasdev( &idum ) ;
    frcz[iat] += sigma * sqrt(masse) * gasdev( &idum ) ;

    /* dernier demi pas vitesse */
    vx[iat] = ( vx[iat] + mass_inv * frcx[iat] ) / ( 1. + 0.5 * dt * xsi ) ;
    vy[iat] = ( vy[iat] + mass_inv * frcy[iat] ) / ( 1. + 0.5 * dt * xsi ) ;
    vz[iat] = ( vz[iat] + mass_inv * frcz[iat] ) / ( 1. + 0.5 * dt * xsi ) ;

  }
	
  return erreur;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * second 1/2 pas vitesse incluant la friction et le deplacement aleatoire   * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int vitesse_langevin( void ) {

  int    erreur = EXIT_SUCCESS ;
  int    iat ;
  double mass_inv, alpha, sigma ;
	
  mass_inv = 0.5 * dt / masse ;

  alpha = exp( - xsi * dt / masse ) ;
  sigma = sqrt( ( 1. - alpha * alpha ) * boltzman * temp0 / masse ) ;

  for ( iat = 0 ; iat < nat ; iat++ ) {
				
    /* dernier demi pas vitesse : partie NVE */
    vx[iat] += mass_inv * frcx[iat] ;
    vy[iat] += mass_inv * frcy[iat] ;
    vz[iat] += mass_inv * frcz[iat] ;

    /* ajout de la friction et de la force aleatoire */
    vx[iat] = alpha * vx[iat] + sigma * gasdev( &idum ) ;
    vy[iat] = alpha * vy[iat] + sigma * gasdev( &idum ) ;
    vz[iat] = alpha * vz[iat] + sigma * gasdev( &idum ) ;
	
  }
	
  return erreur;
}

