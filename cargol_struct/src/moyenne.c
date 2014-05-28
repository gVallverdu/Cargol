
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * calcul de grandeurs moyennes                                              * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"

int moyenne ( int idmethode, Data *d, Etat *e, Particules *Particule[] ) {

  Particules *p = NULL ;

  int     erreur = EXIT_SUCCESS ;
  int     iat ;
  double  inv_temp_int ;
  double  viriel ;
  double  conv ;
  double  ecinet ;
  
// calculs ecinet, energie interne, vitesse globale, temperature interne
  e->ektot = 0. ;
  e->eitot = 0. ;
  inv_temp_int = 0. ;
  e->vgx = e->vgy = e->vgz = 0. ;
  for ( iat = 0 ; iat < e->nat ; iat++ ) {
    p = &((*Particule)[iat]) ;

    // calcul de l'energie cinetique
    e->ektot += 0.5 * d->masse * ( p->vx * p->vx + p->vy * p->vy + p->vz * p->vz ) ;

    // vitesse du centre de gravite
    e->vgx += d->masse * p->vx ;
    e->vgy += d->masse * p->vy ;
    e->vgz += d->masse * p->vz ;

    // energie interne et temperature interne
    if ( idmethode == idDPDE || idmethode == idDPDE_alamain || idmethode == idDPDE_ito ) {
      e->eitot += p->Eint ;
      inv_temp_int += 1. / p->Tint ;
    }
  }
  
// enertie totale
  e->etot = e->ektot + e->eptot ;

// vitesse du centre de gravite
  e->vgx = e->vgx / e->massetot ;
  e->vgy = e->vgy / e->massetot ;
  e->vgz = e->vgz / e->massetot ;
  
// temperature cinetique
  ecinet = e->ektot - 0.5 * e->massetot * ( e->vgx * e->vgx + e->vgy * e->vgy + e->vgz * e->vgz ) ;
  e->temperature = 2. * ecinet / ( 3. * (double) e->nat * d->boltzman ) ;
  
// temperature interne
  if ( idmethode == idDPDE || idmethode == idDPDE_alamain || idmethode == idDPDE_ito ) {
    inv_temp_int /= (double) e->nat ;
  }

// pression
  viriel = e->virielx + e->viriely + e->virielz ;
  e->pression = 2. * e->ektot / ( 3. * e->volume ) + viriel / ( 3. * e->volume ) ;

// impression dans le fichier amdout
  if ( d->mdout == NULL ) stop("erreur fichier mdout ferme") ;
  fprintf( d->mdout, "%14.6e", d->dt * (double) d->numerostep ) ;
  fprintf( d->mdout, "%14.6e", e->etot  / (double) e->nat ) ;
  fprintf( d->mdout, "%14.6e", e->ektot / (double) e->nat ) ;
  fprintf( d->mdout, "%14.6e", e->eptot / (double) e->nat ) ;
  fprintf( d->mdout, "%14.6e", e->temperature ) ;
  fprintf( d->mdout, "%14.6e", e->pression ) ;
  fprintf( d->mdout, "%14.6e", e->vgx ) ;
  fprintf( d->mdout, "%14.6e", e->vgy ) ;
  fprintf( d->mdout, "%14.6e", e->vgz ) ;
  if ( idmethode == idDPDE || idmethode == idDPDE_alamain || idmethode == idDPDE_ito ) {
    fprintf( d->mdout, "%14.6e", 1. / inv_temp_int ) ;
    fprintf( d->mdout, "%14.6e", e->eitot / (double) e->nat ) ;
  }
  fprintf ( d->mdout, "\n" ) ;

// affichage a l'ecran
  if ( ( d->nout != 0 ) && ( d->numerostep % d->nout == 0 || d->numerostep == 0 ) ) {
    printf( "%8d", d->numerostep );
    conv = d->avogadro / ( 1000. * (double) e->nat ) ;
    if ( idmethode == idDPDE || idmethode == idDPDE_alamain || idmethode == idDPDE_ito ) {
      printf("   E = %11.4e"  , ( e->etot + e->eitot )  * conv ) ;
    } else {
      printf("   E = %11.4e"  , e->etot  * conv ) ;
    }
    printf("   Ec = %11.4e" , e->ektot * conv ) ;
    printf("   Ep = %11.4e" , e->eptot * conv ) ;
    printf("   T = %9.3f"   , e->temperature ) ;
    printf("   P = %11.4e"  , e->pression ) ;
    printf("   voisin = %d\n", d->nbremajvoisin ) ;
    d->nbremajvoisin = 0 ;
  }

// calculs des valeurs moyennes et fluctuations
  // temperature
  e->ave_temp += e->temperature ;
  e->sig_temp += e->temperature * e->temperature ;

  // pression
  e->ave_pres += e->pression ;
  e->sig_pres += e->pression * e->pression ;

  // energie totale
  e->ave_etot += e->etot ;
  e->sig_etot += e->etot * e->etot ;

  // energie cinetique totale
  e->ave_ektot += e->ektot ;
  e->sig_ektot += e->ektot * e->ektot ;

  // energie potentielle totale
  e->ave_eptot += e->eptot ;
  e->sig_eptot += e->eptot * e->eptot ;

  if ( idmethode == idDPDE || idmethode == idDPDE_alamain || idmethode == idDPDE_ito ) {
    // energie interne totale
    e->ave_eitot += e->eitot ;
    e->sig_eitot += e->eitot * e->eitot ;

    // temperature interne
    e->ave_temp_int += 1. / inv_temp_int ;
    e->sig_temp_int += 1. / ( inv_temp_int * inv_temp_int ) ;
  }

  return erreur;
}

