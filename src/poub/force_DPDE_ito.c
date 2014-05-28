
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

int force_DPDE_ito( void ) {

  int     erreur = EXIT_SUCCESS ;
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

  double *vxtemp = NULL, *vytemp = NULL, *vztemp = NULL ;
  double  eitot1, ektot1 ;
  double  eitot2, ektot2 ;
  double  scal, racscal ;
  double  dEeps ;
  
  double  ektotx1, ektoty1, ektotz1 ;
  double  ektotx2, ektoty2, ektotz2 ;
  double  rscalx, rscaly, rscalz ;
  double  scalx, scaly, scalz ;
  
  int     ii, jj ;
  long    seed ;

  sigma = sqrt( 2. * xsi * boltzman * temp0 ) ; 	// sqrt( M^2 L^2 T^-3 )
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

  seed = numerostep * nat * nbrevoisinmax + nat * (nat-1) / 2 ;

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
        gamma_ij = 0.5 * xsi * temp0 * ( 1. / temp_interne[iat] + 1. / temp_interne[jat] ) ;


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
	/*
        if ( iat < jat ) {
          ii = iat ;
          jj = jat ;
        } else {
          ii = jat ;
          jj = iat ;
        }
        
        srand( seed - (jj-1)*(jj-2)/2 + ii + 0 );
        wx = gauss() ;
        wy = gauss() ;
        wz = gauss() ;
	*/

        wx = gasdev( &idum ) ;
        wy = gasdev( &idum ) ;
        wz = gasdev( &idum ) ;

        // DPD avec un pas complet sur les forces : on a fait le *dt
        fdpdx = - gamma_ij * chi2 * dvx * dt + sigma * racdt * chi * wx ;
        fdpdy = - gamma_ij * chi2 * dvy * dt + sigma * racdt * chi * wy ;
        fdpdz = - gamma_ij * chi2 * dvz * dt + sigma * racdt * chi * wz ;

        
        
        /* pas de maj des forces : dissipations et fluctuations n'interviennent plus */



        /* variation d'energie interne calcul d'Ito : DPDE */

        dEeps  = fdpdx * dvx + fdpdy * dvy + fdpdz * dvz ;
        dEeps += 3. * sigma * sigma * chi2 * dt / masse ;
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
  ektotx1 = ektoty1 = ektotz1 = 0. ;
  ektotx2 = ektoty2 = ektotz2 = 0. ;  
  for ( iat = 0 ; iat < nat ; iat++ ) {   
    // avant DPD
    ektot1 += 0.5 * masse * ( vx[iat]*vx[iat] + vy[iat]*vy[iat] + vz[iat]*vz[iat] ) ;
    
    ektotx1 += 0.5 * masse * vx[iat]*vx[iat] ;
    ektoty1 += 0.5 * masse * vy[iat]*vy[iat] ;
    ektotz1 += 0.5 * masse * vz[iat]*vz[iat] ;

    // maj des vitesses
    vx[iat] += vxtemp[iat] ;
    vy[iat] += vytemp[iat] ;
    vz[iat] += vztemp[iat] ;

    // apres DPD
    eitot2 += e_interne[iat] ;
    ektot2 += 0.5 * masse * ( vx[iat]*vx[iat] + vy[iat]*vy[iat] + vz[iat]*vz[iat] ) ;
    
    ektotx2 += 0.5 * masse * vx[iat]*vx[iat] ;
    ektoty2 += 0.5 * masse * vy[iat]*vy[iat] ;
    ektotz2 += 0.5 * masse * vz[iat]*vz[iat] ;
  }
  

  // scaling de eitot2
  // scal = (ektot1 + eitot1 - ektot2 ) / eitot2 ;

  // scaling isotrope sans eptot 
  scal = (ektot1 + eitot1) / (ektot2 + eitot2) ;

  // iso
  // scalx = scaly = scalz = scal ;
  
  // aniso
  scalx = (ektotx1 + (eitot1-eitot2)/3.) / ektotx2 ;
  scaly = (ektoty1 + (eitot1-eitot2)/3.) / ektoty2 ;  
  scalz = (ektotz1 + (eitot1-eitot2)/3.) / ektotz2 ;

  racscal = 0. ;
  if ( scal > 0. ) {
  	racscal = sqrt(scal) ;
  } else {
  	stop("DDPE, scaling de ektot2 : scal < 0") ;
  }

  rscalx = rscaly = rscalz = 0. ;
  if ( scalx > 0. ) {
    rscalx = sqrt( scalx ) ;
  } else {
    stop("DDPE, scaling de l'energie : scalX < 0") ;
  }
  if ( scaly > 0. ) {
    rscaly = sqrt( scaly ) ;
  } else {
    stop("DDPE, scaling de l'energie : scalY < 0") ;
  }
  if ( scalz > 0. ) {
    rscalz = sqrt( scalz ) ;
  } else {
    stop("DDPE, scaling de l'energie : scalZ < 0") ;
  }

  // scaling des vitesses et de l'energie interne
  ektot = 0. ;
  for ( iat = 0 ; iat < nat ; iat++ ) {
  
    // e_interne[iat] = scal * e_interne[iat] ;
    
    vx[iat] = rscalx * vx[iat] ;
    vy[iat] = rscaly * vy[iat] ;
    vz[iat] = rscalz * vz[iat] ;
        
    ektot += 0.5 * masse * ( vx[iat]*vx[iat] + vy[iat]*vy[iat] + vz[iat]*vz[iat] ) ;
    
    // maj de la temperature interne
    temp_interne[iat] = e_interne[iat] / capacite_calo ;

    if( temp_interne[iat] < 0. ) stop("temperature et energie interne < 0") ;
  }
  
  free(vxtemp) ;
  free(vytemp) ;
  free(vztemp) ;
  
  /* * * * * * * * * * * * * * * * * * * * * *
   *                                         *
   *  impression des energies internes       *
   *                                         *
   * * * * * * * * * * * * * * * * * * * * * */

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

