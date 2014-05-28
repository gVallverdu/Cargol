
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * DPD energie constante :                                                   * 
 * force dissipative, force aleatoire, energie et temperature interne        * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"


static int force_DPDE_alamain( int, Data *, Particules ** ) ;
static int force_DPDE_ito( int, Data *, Particules ** ) ;

int force_DPDE( int idmethode, int Natom, Data *Donnees, Particules *Particule[] ) {

  Particules *pi = NULL ;

  int erreur = EXIT_SUCCESS ;
  int iat ;
  
  if ( idmethode == idDPDE || idmethode == idDPDE_alamain ) {
    force_DPDE_alamain( Natom, Donnees, Particule ) ;
            
  } else if ( idmethode == idDPDE_ito ) {
    force_DPDE_ito( Natom, Donnees, Particule ) ;

  } else {
    return erreur ;
  }

  /* impression des temperatures internes */
  if ( Donnees->freqTint != 0 && Donnees->numerostep % Donnees->freqTint == 0 ) {
    fprintf( Donnees->ftemp_int, "& iteration %d \n", Donnees->numerostep ) ;
    for ( iat = 0 ; iat < Natom ; iat++ ) {
      pi = &((*Particule)[iat]) ;
      
      fprintf( Donnees->ftemp_int ,"%5d ",iat ) ;
      fprintf( Donnees->ftemp_int ,"%16.7e " , pi->Tint ) ;
      fprintf( Donnees->ftemp_int ,"%16.7e\n", pi->Eint * Donnees->avogadro / 1000. ) ;
    }
  }

  return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * calcul direct de la variation d'energie interne                           * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int force_DPDE_alamain( int Natom, Data *d, Particules *Particule[] ) {

  Particules *pi = NULL, *pj = NULL ;

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
  double  Eci_old, Ecj_old, Eci, Ecj, dEci, dEcj ;
  double  dEeps ;
  double  masse_inv = 1. / d->masse ;
  double  dt = d->dt ;
  
  sigma2 = d->sigmaDPDE * d->sigmaDPDE ; 		// sqrt( M^2 L^2 T^-3 )
  racdt = sqrt( dt ) ;
  
  for ( iat = 0 ; iat < Natom-1 ; iat++ ) {
    pi = &((*Particule)[iat]) ;

    nvois = pi->nbrevoisin ;
    for ( ivois = 0 ; ivois < nvois ; ivois++ ) {

      jat = pi->listevoisin[ivois] ;
      pj = &((*Particule)[jat]) ;

      // vecteur i -> j
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

        // friction pour la paire ij
        gamma_ij = 0.5 * sigma2 * ( 1. / pi->Tint + 1. / pj->Tint ) / d->boltzman ;

        // sauvegarde energie cinetique de i et j avant fluctuation/dissipation
        Eci_old = 0.5 * d->masse * ( pi->vx * pi->vx + pi->vy * pi->vy + pi->vz * pi->vz ) ;
        Ecj_old = 0.5 * d->masse * ( pj->vx * pj->vx + pj->vy * pj->vy + pj->vz * pj->vz ) ;

        
        /* modification des forces pour la DPD */

        // chi (sans unite) : fonction de cutt off simple = lineaire
        dist = sqrt(dist2) ;
        chi = 1. - dist / d->rcut ;
        chi2 = chi * chi ;

        // vitesse relative
        dvx = ( pi->vx - pj->vx ) ;
        dvy = ( pi->vy - pj->vy ) ;
        dvz = ( pi->vz - pj->vz ) ;

        // tirage
        wx = gasdev( &(d->idum) ) ;
        wy = gasdev( &(d->idum) ) ;
        wz = gasdev( &(d->idum) ) ;
        
        // DPD avec un pas complet sur les forces : on a fait le *dt
        fdpdx = - gamma_ij * chi2 * dvx * dt + d->sigmaDPDE * racdt * chi * wx ;
        fdpdy = - gamma_ij * chi2 * dvy * dt + d->sigmaDPDE * racdt * chi * wy ;
        fdpdz = - gamma_ij * chi2 * dvz * dt + d->sigmaDPDE * racdt * chi * wz ;

        // pas de maj des forces : dissipations et fluctuations n'interviennent plus

        // maj des vitesses dues aux forces dissipatives et aleatoires
        fdpdx = fdpdx * masse_inv ;
        fdpdy = fdpdy * masse_inv ;
        fdpdz = fdpdz * masse_inv ;

        pi->vx += fdpdx ;
        pi->vy += fdpdy ;
        pi->vz += fdpdz ;

        pj->vx -= fdpdx ;
        pj->vy -= fdpdy ;
        pj->vz -= fdpdz ;

        
        /* variation energie interne : DPDE */

        // variation d'energie cinetique "a la main"
        Eci = 0.5 * d->masse * ( pi->vx*pi->vx + pi->vy*pi->vy + pi->vz*pi->vz ) ;
        Ecj = 0.5 * d->masse * ( pj->vx*pj->vx + pj->vy*pj->vy + pj->vz*pj->vz ) ;

        dEci = Eci - Eci_old ;
        dEcj = Ecj - Ecj_old ;
                  
        dEeps = - 0.5 * ( dEci + dEcj ) ;

        // maj de l'energie interne
        pi->Eint += dEeps ;
        pj->Eint += dEeps ;
        
      }
    } // voisin
  } // iat

  /* maj de la temperature interne */
  for ( iat = 0 ; iat < Natom ; iat++ ) {
    pi = &((*Particule)[iat]) ;
    pi->Tint = pi->Eint / d->capacite_calo ;
    if( pi->Tint < 0. ) stop("temperature et energie interne < 0") ;
  }

  return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * calcul de la variation d'energie interne par la formule d'ito             * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int force_DPDE_ito( int Natom, Data *d, Particules *Particule[] ) {

  Particules *pi = NULL, *pj = NULL ;
  
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
  double  masse_inv = 1. / d->masse ;

  double *vxtemp = NULL, *vytemp = NULL, *vztemp = NULL ;
  double  eitot1, ektot1 ;
  double  eitot2, ektot2 ;
  double  scal ;
  double  dEeps ;
  double  dt = d->dt ;

  sigma2 = d->sigmaDPDE * d->sigmaDPDE ;	 		// sqrt( M^2 L^2 T^-3 )
  racdt = sqrt( dt ) ;

  // allocation tableaux des vitesses temporaires
  vxtemp = calloc( Natom, sizeof(double) ) ;
  vytemp = calloc( Natom, sizeof(double) ) ;
  vztemp = calloc( Natom, sizeof(double) ) ;
  
  // mise a zero des tableaux des vitesses temporaires  
  // calcul de eitot1 pour le scaling de l'energie
  eitot1 = 0. ;
  for ( iat = 0 ; iat < Natom ; iat++ ) {
    vxtemp[iat] = 0. ;
    vytemp[iat] = 0. ;
    vztemp[iat] = 0. ;
    
    pi = &((*Particule)[iat]) ;
    eitot1 += pi->Eint ;
  }


  /* * * * * * * * * * * * * * * * * * * * * *
   *                                         *
   *     boucle sur les paires d'atomes      *
   *                                         *
   * * * * * * * * * * * * * * * * * * * * * */

  for ( iat = 0 ; iat < Natom-1 ; iat++ ) {
    pi = &((*Particule)[iat]) ;

    nvois = pi->nbrevoisin ;
    for ( ivois = 0 ; ivois < nvois ; ivois++ ) {

      jat = pj->listevoisin[ivois] ;
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

        // friction pour la paire ij
        gamma_ij = 0.5 * sigma2 * ( 1. / pi->Tint + 1. / pj->Tint ) / d->boltzman ;

        // chi (sans unite) : fonction de cutt off simple = lineaire
        dist = sqrt(dist2) ;
        chi = 1. - dist / d->rcut ;
        chi2 = chi * chi ;

        // vitesse relative
        dvx = pi->vx - pj->vx ;
        dvy = pi->vy - pj->vy ;
        dvz = pi->vz - pj->vz ;

        wx = gasdev( &(d->idum) ) ;
        wy = gasdev( &(d->idum) ) ;
        wz = gasdev( &(d->idum) ) ;

        // DPD avec un pas complet sur les forces : on a fait le *dt
        fdpdx = - gamma_ij * chi2 * dvx * dt + d->sigmaDPDE * racdt * chi * wx ;
        fdpdy = - gamma_ij * chi2 * dvy * dt + d->sigmaDPDE * racdt * chi * wy ;
        fdpdz = - gamma_ij * chi2 * dvz * dt + d->sigmaDPDE * racdt * chi * wz ;

        
        /* pas de maj des forces : dissipations et fluctuations n'interviennent plus */


        /* variation d'energie interne calcul d'Ito : DPDE */

        dEeps  = fdpdx * dvx + fdpdy * dvy + fdpdz * dvz ;
        dEeps += 3. * sigma2 * chi2 * dt / d->masse ;
        dEeps = - 0.5 * dEeps ;

        // maj de l'energie interne
        pi->Eint += dEeps ;
        pj->Eint += dEeps ;
	
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
  for ( iat = 0 ; iat < Natom ; iat++ ) {
    pi = &((*Particule)[iat]) ;

    // energie cinetique avant fluctuations/dissipation
    ektot1 += 0.5 * d->masse * ( pi->vx*pi->vx + pi->vy*pi->vy + pi->vz*pi->vz ) ;

    // maj des vitesses
    pi->vx += vxtemp[iat] ;
    pi->vy += vytemp[iat] ;
    pi->vz += vztemp[iat] ;

    // energie cinetique apres fluctuations/dissipation
    ektot2 += 0.5 * d->masse * ( pi->vx*pi->vx + pi->vy*pi->vy + pi->vz*pi->vz ) ;

     // energie interne totale apres fluctuation/dissipation
    eitot2 += pi->Eint ;

  }
  
  // scaling de eitot2 uniquement	
  scal = (ektot1 - ektot2 + eitot1 ) / eitot2 ;
  
  // scaling de l'energie interne
  for ( iat = 0 ; iat < Natom ; iat++ ) {
    pi = &((*Particule)[iat]) ;
  
    pi->Eint = scal * pi->Eint ;
    
    // maj de la temperature interne
    pi->Tint = pi->Eint / d->capacite_calo ;
    
    if( pi->Tint < 0. ) stop("temperature et energie interne < 0") ;
  }
  
  free(vxtemp) ;
  free(vytemp) ;
  free(vztemp) ;
  
  return erreur ;
}

