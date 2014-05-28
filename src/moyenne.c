
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * calcul de grandeurs moyennes                                              * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"
#include "DPDE.h"
#include "fichier.h"

int moyenne ( int idmethode ) {

  int     erreur = EXIT_SUCCESS ;
  int     iat ;
  double  inv_temp_int ;
  double  viriel ;
  double  conv ;
  double  ecinet ;
  
// calculs ecinet, energie interne, vitesse globale, temperature interne
  ektot = 0. ;
  eitot = 0. ;
  inv_temp_int = 0. ;
  vgx = vgy = vgz = 0. ;
  for ( iat = 0 ; iat < nat ; iat++ ) {

    // calcul de l'energie cinetique
    ektot += 0.5 * masse * ( vx[iat]*vx[iat] + vy[iat]*vy[iat] + vz[iat]*vz[iat] ) ;

    // vitesse du centre de gravite
    vgx += masse * vx[iat] ;
    vgy += masse * vy[iat] ;
    vgz += masse * vz[iat] ;

    // energie interne et temperature interne
    if ( idmethode == idDPDE || idmethode == idDPDE_alamain || idmethode == idDPDE_ito ) {
      eitot += e_interne[iat] ;
      inv_temp_int += 1. / temp_interne[iat] ;
    }
  }
  
// enertie totale
  etot = ektot + eptot ;

// vitesse du centre de gravite
  vgx = vgx / massetot ;
  vgy = vgy / massetot ;
  vgz = vgz / massetot ;
  
// temperature cinetique
  ecinet = ektot - 0.5 * massetot * ( vgx*vgx + vgy*vgy + vgz*vgz ) ;
  temperature = 2. * ecinet / ( 3. * (double) nat * boltzman ) ;
  
// temperature interne
  if ( idmethode == idDPDE || idmethode == idDPDE_alamain || idmethode == idDPDE_ito ) {
    inv_temp_int /= (double) nat ;
  }

// pression
  viriel = virielx + viriely + virielz ;
  pression = 2. * ektot / ( 3. * volume ) + viriel / ( 3. * volume ) ;

// impression dans le fichier amdout
  if ( mdout == NULL ) erreur++ ;
  fprintf ( mdout, "%14.6e", dt * (double) numerostep ) ;
  fprintf ( mdout, "%14.6e", etot  / (double) nat ) ;
  fprintf ( mdout, "%14.6e", ektot / (double) nat ) ;
  fprintf ( mdout, "%14.6e", eptot / (double) nat ) ;
  fprintf ( mdout, "%14.6e", temperature ) ;
  fprintf ( mdout, "%14.6e", pression ) ;
  fprintf ( mdout, "%14.6e", vgx ) ;
  fprintf ( mdout, "%14.6e", vgy ) ;
  fprintf ( mdout, "%14.6e", vgz ) ;
  if ( idmethode == idDPDE || idmethode == idDPDE_alamain || idmethode == idDPDE_ito ) {
    fprintf ( mdout, "%14.6e", 1. / inv_temp_int ) ;
    fprintf ( mdout, "%14.6e", eitot / (double) nat ) ;
  }
  fprintf ( mdout, "\n" ) ;

// affichage a l'ecran
  if ( ( nout != 0 ) && ( numerostep % nout == 0 || numerostep == 0 ) ) {
    printf( "%8d", numerostep );
    conv = avogadro / ( 1000. * (double) nat ) ;
    if ( idmethode == idDPDE || idmethode == idDPDE_alamain || idmethode == idDPDE_ito ) {
      printf("   E = %11.4e"  , ( etot + eitot )  * conv ) ;
    } else {
      printf("   E = %11.4e"  , etot  * conv ) ;
    }
    printf("   Ec = %11.4e" , ektot * conv ) ;
    printf("   Ep = %11.4e" , eptot * conv ) ;
    printf("   T = %9.3f"   , temperature ) ;
    printf("   P = %11.4e"  , pression ) ;
    printf("   voisin = %d\n", nbremajvoisin ) ;
    nbremajvoisin = 0 ;
  }

// calculs des valeurs moyennes et fluctuations
  // temperature
  ave_temp += temperature ;
  sig_temp += temperature * temperature ;

  // pression
  ave_pres += pression ;
  sig_pres += pression * pression ;

  // energie totale
  ave_etot += etot ;
  sig_etot += etot * etot ;

  // energie cinetique totale
  ave_ektot += ektot ;
  sig_ektot += ektot * ektot ;

  // energie potentielle totale
  ave_eptot += eptot ;
  sig_eptot += eptot * eptot ;

  if ( idmethode == idDPDE || idmethode == idDPDE_alamain || idmethode == idDPDE_ito ) {
    // energie interne totale
    ave_eitot += eitot ;
    sig_eitot += eitot * eitot ;

    // temperature interne
    ave_temp_int += 1. / inv_temp_int ;
    sig_temp_int += 1. / ( inv_temp_int * inv_temp_int ) ;
  }

  return erreur;
}

