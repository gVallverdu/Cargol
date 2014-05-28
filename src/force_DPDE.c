
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * forces DPD dissipative : force de friction, force aleatoire               * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"
#include "DPDE.h"
#include "fichier.h"

static int force_DPDE_alamain( void ) ;
static int force_DPDE_ito( void ) ;

int force_DPDE( int idmethode ) {

  int erreur = EXIT_SUCCESS ;
  int iat ;
  
  if ( idmethode == idDPDE || idmethode == idDPDE_alamain ) {
    force_DPDE_alamain() ;
            
  } else if ( idmethode == idDPDE_ito ) {
    force_DPDE_ito() ;

  } else {
    return erreur ;
  }

  /* impression des temperatures internes */
  if ( freqTint != 0 && numerostep % freqTint == 0 ) {
    fprintf( ftemp_int, "& iteration %d\n", numerostep ) ;
    for ( iat = 0 ; iat < nat ; iat++ ) {
      fprintf( ftemp_int ,"%5d ",iat ) ;
      fprintf( ftemp_int ,"%16.7e " , temp_interne[iat] ) ;
      fprintf( ftemp_int ,"%16.7e\n", e_interne[iat] * avogadro / 1000. ) ;
    }
  }

  return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * calcul direct de la variation d'energie interne                           * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int force_DPDE_alamain( void ) {

  int     erreur = EXIT_SUCCESS ;
  int     iat, jat, ivois, nvois ;
  double  xij, yij, zij ;
  double  dist, dist2 ;
  double  sigma2, racdt ;
  double  gamma_ij ;
  double  chi, chi2 ;
  double  dvx, dvy, dvz ;
  double  fdpdx, fdpdy, fdpdz ;
  double  wx, wy, wz ;
  double  masse_inv = 1. / masse ;
  double  Eci_old, Ecj_old, Eci, Ecj, dEci, dEcj ;
  double  dEeps ;

  sigma2 = sigmaDPDE * sigmaDPDE ;  	// sqrt( M^2 L^2 T^-3 )
  racdt = sqrt( dt ) ;
  
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
        gamma_ij = 0.25 * sigma2 * ( 1. / temp_interne[iat] + 1. / temp_interne[jat] ) / boltzman ;

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
        wx = gasdev( &idum ) ;
        wy = gasdev( &idum ) ;
        wz = gasdev( &idum ) ;
        
        // DPD avec un pas complet sur les forces : on a fait le *dt
        fdpdx = - gamma_ij * chi2 * dvx * dt + sigmaDPDE * racdt * chi * wx ;
        fdpdy = - gamma_ij * chi2 * dvy * dt + sigmaDPDE * racdt * chi * wy ;
        fdpdz = - gamma_ij * chi2 * dvz * dt + sigmaDPDE * racdt * chi * wz ;

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
    } 
  } 

  /* maj de la temperature interne */
  for ( iat = 0 ; iat < nat ; iat++ ) {
    temp_interne[iat] = e_interne[iat] / capacite_calo ;
    if( temp_interne[iat] < 0. ) stop("temperature et energie interne < 0") ;
  }

  return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * calcul de la variation d'energie interne par la formule d'ito             * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int force_DPDE_ito( void ) {

  int     erreur = EXIT_SUCCESS ;
  int     iat, jat, ivois, nvois ;
  double  xij, yij, zij ;
  double  dist, dist2 ;
  double  sigma2, racdt ;
  double  gamma_ij ;
  double  chi, chi2 ;
  double  dvx, dvy, dvz ;
  double  fdpdx, fdpdy, fdpdz ;
  double  wx, wy, wz ;
  double  masse_inv = 1. / masse ;
  double  *vxtemp = NULL, *vytemp = NULL, *vztemp = NULL ;
  double  eitot1, ektot1 ;
  double  eitot2, ektot2 ;
  double  scal ;
  double  dEeps ;

  sigma2 = sigmaDPDE * sigmaDPDE ;  	// sqrt( M^2 L^2 T^-3 )
  racdt = sqrt( dt ) ;
  
  // allocation tableaux des vitesses temporaires
  vxtemp = calloc( nat, sizeof(double) ) ;
  vytemp = calloc( nat, sizeof(double) ) ;
  vztemp = calloc( nat, sizeof(double) ) ;
  
  // mise a zero des tableaux des vitesses temporaires  
  // calcul de eitot1 pour le scaling de l'energie
  eitot1 = 0. ;
  for ( iat = 0 ; iat < nat ; iat++ ) {
    vxtemp[iat] = 0. ;
    vytemp[iat] = 0. ;
    vztemp[iat] = 0. ;
    
    eitot1 += e_interne[iat] ;
  }

  /* * * * * * * * * * * * * * * * * * * * * *
   *                                         *
   *     boucle sur les paires d'atomes      *
   *                                         *
   * * * * * * * * * * * * * * * * * * * * * */

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
        gamma_ij = 0.25 * sigma2 * ( 1. / temp_interne[iat] + 1. / temp_interne[jat] ) / boltzman ;

        // chi (sans unite) : fonction de cutt off simple = lineaire
        dist = sqrt(dist2) ;
        chi = 1. - dist / rcut ;
        chi2 = chi * chi ;

        // vitesse relative
        dvx = ( vx[iat] - vx[jat] ) ;
        dvy = ( vy[iat] - vy[jat] ) ;
        dvz = ( vz[iat] - vz[jat] ) ;

        // tirage
        wx = gasdev( &idum ) ;
        wy = gasdev( &idum ) ;
        wz = gasdev( &idum ) ;

        // DPD avec un pas complet sur les forces : on a fait le *dt
        fdpdx = - gamma_ij * chi2 * dvx * dt + sigmaDPDE * racdt * chi * wx ;
        fdpdy = - gamma_ij * chi2 * dvy * dt + sigmaDPDE * racdt * chi * wy ;
        fdpdz = - gamma_ij * chi2 * dvz * dt + sigmaDPDE * racdt * chi * wz ;

        
        /* pas de maj des forces : dissipations et fluctuations n'interviennent plus */


        /* variation d'energie interne calcul d'Ito : DPDE */

        dEeps  = fdpdx * dvx + fdpdy * dvy + fdpdz * dvz ;
        dEeps += 3. * sigma2 * chi2 * dt / masse ;
        dEeps = - 0.5 * dEeps ;

        // maj de l'energie interne
        e_interne[iat] += dEeps ;
        e_interne[jat] += dEeps ;
	
        /* maj temporaires des vitesses */
        fdpdx = fdpdx * masse_inv ;
        fdpdy = fdpdy * masse_inv ;
        fdpdz = fdpdz * masse_inv ;

        vxtemp[iat] += fdpdx ;
        vytemp[iat] += fdpdy ;
        vztemp[iat] += fdpdz ;

        vxtemp[jat] -= fdpdx ;
        vytemp[jat] -= fdpdy ;
        vztemp[jat] -= fdpdz ;

      }
    } 
  }

  /* * * * * * * * * * * * * * * * * * * * * *
   *                                         *
   *     maj vitesse et scaling energie      *
   *                                         *
   * * * * * * * * * * * * * * * * * * * * * */

  eitot2 = 0. ;
  ektot1 = 0. ;
  ektot2 = 0. ;
  for ( iat = 0 ; iat < nat ; iat++ ) {

    // energie cinetique avant fluctuations/dissipation
    ektot1 += 0.5 * masse * ( vx[iat]*vx[iat] + vy[iat]*vy[iat] + vz[iat]*vz[iat] ) ;

    // maj des vitesses
    vx[iat] += vxtemp[iat] ;
    vy[iat] += vytemp[iat] ;
    vz[iat] += vztemp[iat] ;

    // energie cinetique apres fluctuations/dissipation
    ektot2 += 0.5 * masse * ( vx[iat]*vx[iat] + vy[iat]*vy[iat] + vz[iat]*vz[iat] ) ;

     // energie interne totale apres fluctuation/dissipation
    eitot2 += e_interne[iat] ;

  }
  
  // scaling de eitot2 uniquement	
  scal = (ektot1 - ektot2 + eitot1 ) / eitot2 ;
  
  // scaling de l'energie interne
  for ( iat = 0 ; iat < nat ; iat++ ) {
  
    e_interne[iat] = scal * e_interne[iat] ;
    
    // maj de la temperature interne
    temp_interne[iat] = e_interne[iat] / capacite_calo ;
    
    if( temp_interne[iat] < 0. ) stop("temperature et energie interne < 0") ;
  }
  
  free(vxtemp) ;
  free(vytemp) ;
  free(vztemp) ;
  
  return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
