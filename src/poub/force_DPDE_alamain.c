
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * DPD energie constante :                                                   * 
 * force dissipative, force aleatoire, energie et temperature interne        * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"
#include "DPDE.h"
#include "fichier.h"

extern double gasdev( long * ) ;
extern double gauss( void ) ;
extern double ran0( long * ) ;

int force_DPDE_alamain( void ) {

  int     erreur = 0 ;
  int     iat, jat, ivois, nvois ;
  double  xij, yij, zij ;
  double  dist, dist2 ;

  double  sigma, racdt ;
  double  gamma_ij ;
  double  chi, chi2 ;
  double  dvx, dvy, dvz ;
  double  fdpdx, fdpdy, fdpdz ;
  double  wx, wy, wz ;

  double  masse_inv = 1. / masse ;

  double  Eci_old, Ecj_old, Eci, Ecj, dEci, dEcj ;
  double  dEeps ;

  strcpy( nomfonction, "force_DPDE_alamain()" ) ;

  sigma = sqrt( 2. * xsi * boltzman * temp0 ) ; 	// sqrt( M^2 L^2 T^-3 )
  racdt = sqrt( dt ) ;
  
  // ordre aleatoire des atomes
  int k, dejavu, i ;
  int liste[1000] ;
  k = 0 ;
  while ( k != nat ) {
    iat = (int) ( nat * ran0(&idum) ) ;
    
    dejavu = 0 ;
    for ( i = 0 ; i < k ; i++ ) {
      if ( liste[i] == iat ) dejavu++ ;
    }
      
    if ( dejavu == 0 ) {
      liste[k] = iat ;
      k++ ;
        
    } else if ( dejavu == 1 ) {
      continue ;
        
    } else if ( dejavu > 1 ) {
      printf("%d %d \n", k, iat );
      for ( i = 0 ; i < k ; i++ ) printf("%3d %4d\n", i, liste[i] ) ;
      stop("on a enregistre plusieurs fois le meme") ;
    }
      
  // for ( iat = 0 ; iat < nat-1 ; iat++ ) {

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

        // friction pour la paire ij
        gamma_ij = 0.5 * xsi * temp0 * ( 1. / temp_interne[iat] + 1. / temp_interne[jat] ) ;

        // sauvegarde energie cinetique de i et j avant fluctuation/dissipation
        Eci_old = 0.5 * masse * ( vx[iat]*vx[iat] + vy[iat]*vy[iat] + vz[iat]*vz[iat] ) ;
        Ecj_old = 0.5 * masse * ( vx[jat]*vx[jat] + vy[jat]*vy[jat] + vz[jat]*vz[jat] ) ;

        
        
        /* modification des forces pour la DPD */

        // chi (sans unite) : fonction de cutt off simple = lineaire
        dist = sqrt(dist2) ;
        chi = 1. - dist / rcut ;
        chi2 = chi * chi ;

        // vitesse relative
        dvx = ( vx[iat] - vx[jat] ) ;
        dvy = ( vy[iat] - vy[jat] ) ;
        dvz = ( vz[iat] - vz[jat] ) ;

        // tirage
        wx = wxrand[iat][jat] ;
        wy = wyrand[iat][jat] ;
        wz = wzrand[iat][jat] ;
        /*
        wx = gauss() ;
        wy = gauss() ;
        wz = gauss() ;
        */
        /*
        wx = gasdev( &idum ) ;
        wy = gasdev( &idum ) ;
        wz = gasdev( &idum ) ;
        */
        
        // DPD avec un pas complet sur les forces : on a fait le *dt
        fdpdx = - gamma_ij * chi2 * dvx * dt + sigma * racdt * chi * wx ;
        fdpdy = - gamma_ij * chi2 * dvy * dt + sigma * racdt * chi * wy ;
        fdpdz = - gamma_ij * chi2 * dvz * dt + sigma * racdt * chi * wz ;

        // pas de maj des forces : dissipations et fluctuations n'interviennent plus

        // maj des vitesses dues aux forces dissipatives et aleatoires
        fdpdx = fdpdx * masse_inv ;
        fdpdy = fdpdy * masse_inv ;
        fdpdz = fdpdz * masse_inv ;

        vx[iat] += fdpdx ;
        vy[iat] += fdpdy ;
        vz[iat] += fdpdz ;

        vx[jat] -= fdpdx ;
        vy[jat] -= fdpdy ;
        vz[jat] -= fdpdz ;

                
        
        /* variation energie interne : DPDE */

        // variation d'energie cinetique "a la main"
        Eci = 0.5 * masse * ( vx[iat]*vx[iat] + vy[iat]*vy[iat] + vz[iat]*vz[iat] ) ;
        Ecj = 0.5 * masse * ( vx[jat]*vx[jat] + vy[jat]*vy[jat] + vz[jat]*vz[jat] ) ;

        dEci = Eci - Eci_old ;
        dEcj = Ecj - Ecj_old ;
                  
        dEeps = - 0.5 * ( dEci + dEcj ) ;

        // maj de l'energie interne
        e_interne[iat] += dEeps ;
        e_interne[jat] += dEeps ;
        
      }
    } // voisin
  } // iat


  /* maj de la temperature interne */
  for ( iat = 0 ; iat < nat ; iat++ ) {
    temp_interne[iat] = e_interne[iat] / capacite_calo ;
    if( temp_interne[iat] < 0. ) stop("temperature et energie interne < 0") ;
  }


  /* impression des temperatures internes */
  if ( ntemp_int != 0 && numerostep % ntemp_int == 0 ) {
    fprintf( ftemp_int, "&\n") ;
    for ( iat = 0 ; iat < nat ; iat++ ) {
      fprintf( ftemp_int ,"%5d ",iat ) ;
      fprintf( ftemp_int ,"%16.7e " , temp_interne[iat] ) ;
      fprintf( ftemp_int ,"%16.7e\n", e_interne[iat]*avogadro/1000. ) ;
    }
  }

  return erreur ;
}

