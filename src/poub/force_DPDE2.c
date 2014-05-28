/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* DPD energie constante :                                                   */
/* force dissipative, force aleatoire, energie et temperature interne        */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

extern	double	gaussienne( void ) ;

int force_DPDE( void ) {

  int  erreur = 0 ;

  int iat, jat, ivois, nvois ;
  double xij, yij, zij ;
  double dist, dist2 ;

  double sigma, racdt ;
  double chi, chi2 ;
  double dvx, dvy, dvz ;
  double fdpdx, fdpdy, fdpdz ;
  double wx, wy, wz ;

  double masse_inv ;

  double dEeps, var_temp ;
  double gamma_ij ;

  sigma = sqrt( 2. * xsi * boltzman * temp0 ) ; 	// sqrt( M^2 L^2 T^-3 )
  racdt = sqrt( dt ) ;

  // masse_inv = dt / masse ;
  masse_inv = 1. / masse ;

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

        // friction pour la paire ij
        gamma_ij = 0.5 * xsi * temp0 * ( 1. / temp_interne[iat] + 1. / temp_interne[jat] ) ;

/* * * * * * * *  modification des forces pour la DPD  * * * * * * * * */

        // chi (sans unite) : fonction de cutt off simple = lineaire
        dist = sqrt(dist2) ;
        chi = 1. - dist / rcut ;
        chi2 = chi * chi ;

        // vitesse relative
        dvx = ( vx[iat] - vx[jat] ) ;
        dvy = ( vy[iat] - vy[jat] ) ;
        dvz = ( vz[iat] - vz[jat] ) ;

        // tirage
        wx = gaussienne() ;
        wy = gaussienne() ;
        wz = gaussienne() ;

        // DPD avec un pas complet sur les forces : on a fait le *dt
        fdpdx = - gamma_ij * chi2 * dvx * dt + sigma * racdt * chi * wx ;
        fdpdy = - gamma_ij * chi2 * dvy * dt + sigma * racdt * chi * wy ;
        fdpdz = - gamma_ij * chi2 * dvz * dt + sigma * racdt * chi * wz ;

        // pas de maj des forces : dissipations et fluctuations n'interviennent plus

        // variation d'energie interne : DPDE 
        dEeps  = fdpdx * dvx + fdpdy * dvy + fdpdz * dvz ;
        dEeps += 3. * sigma * sigma * chi2 * dt / masse ;

        dEeps = - 0.5 * dEeps ;

        // maj de l'energie interne
        e_interne[iat] += dEeps ;
        e_interne[jat] += dEeps ;

        // maj de la temperature interne
        var_temp = dEeps / capacite_calo ;
        temp_interne[iat] += var_temp ;
        temp_interne[jat] += var_temp ;

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
      }

    } // voisin

  } // iat

  return erreur ;

}

/* calcul exact avec la formule d'Ito :

        // tire du code de gabriel stamp2 : computeForceDPD.c

        // DPD avec un pas complet sur les forces : on a fait le *dt
        fdpdx = - gamma_ij * chi2 * dvx * dt + sigma * racdt * chi * wx ;
        fdpdy = - gamma_ij * chi2 * dvy * dt + sigma * racdt * chi * wy ;
        fdpdz = - gamma_ij * chi2 * dvz * dt + sigma * racdt * chi * wz ;

        denergie  = fdpdx * dvx + fdpdy * dvy + fdpdz * dvz ;
        // sur ces termes faire *3 car le terme est pour x y et z ?
        denergie += sigma * sigma * chi2 * dt / masse ;
        // si iat et jat n'ont pas la même masse :
        denergie += 0.5 * ( 1./masse[iat] + 1./masse[jat] ) * sigma * sigma * chi2 * dt  ;

*/

