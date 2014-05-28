
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * second 1/2 pas vitesse incluant la friction et le deplacement aleatoire   * 
 *                                                                           * 
 * algorithme de verlet vitesse                                              * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

extern double gasdev ( long * ) ;
extern double ran0( long * ) ;
extern double ran0bis( long * ) ;
extern double gasmin( long * );
extern double random_number( void ) ;
extern double gauss( void ) ;
extern double gauss0( long * ) ;
extern double gasdevC ( void) ;

int vitesse_langevin ( void ) {

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
    srand( numerostep * nat + 3 * iat + 0 );
    vx[iat] = alpha * vx[iat] + sigma * gauss() ;

//    srand( numerostep * nat + 3 * iat + 1 );        
    vy[iat] = alpha * vy[iat] + sigma * gauss() ;
    
//    srand( numerostep * nat + 3 * iat + 2 );
    vz[iat] = alpha * vz[iat] + sigma * gauss() ;
    
    
    /*
    srand( numerostep * nat + 3 * iat + 0 );
    vx[iat] = alpha * vx[iat] + sigma * gasdevC() ;

    srand( numerostep * nat + 3 * iat + 1 );        
    vy[iat] = alpha * vy[iat] + sigma * gasdevC() ;
    
    srand( numerostep * nat + 3 * iat + 2 );
    vz[iat] = alpha * vz[iat] + sigma * gasdevC() ;
    */
    
    /*
    idum = (long) numerostep * nat + 3 * iat + 0 ;
    vx[iat] = alpha * vx[iat] + sigma * gauss0( &idum ) ;

    idum = (long) numerostep * nat + 3 * iat + 1 ;
    vy[iat] = alpha * vy[iat] + sigma * gauss0( &idum ) ;
    
    idum = (long) numerostep * nat + 3 * iat + 2 ;
    vz[iat] = alpha * vz[iat] + sigma * gauss0( &idum ) ;
    */
        
    // ajout de la friction et de la force aleatoire
    /*
    vx[iat] = alpha * vx[iat] + sigma * gasdev( &idum ) ;
    vy[iat] = alpha * vy[iat] + sigma * gasdev( &idum ) ;
    vz[iat] = alpha * vz[iat] + sigma * gasdev( &idum ) ;
    */
    
  }
	
  return erreur;
}

