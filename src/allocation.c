/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * allocation des tableaux dynamique : forces, positions, vitesses           * 
 *                                                                           * 
 * initialistion a 0 de tous les tableaux                                    * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"
#include "DPDE.h"

int allocation( void ) {

  int erreur = EXIT_SUCCESS ;
  int iat, ivois, ix ;
  
// forces
  frcx = calloc( nat, sizeof(double) ) ;
  frcy = calloc( nat, sizeof(double) ) ;
  frcz = calloc( nat, sizeof(double) ) ;
	
// positions
  x = calloc( nat, sizeof(double) ) ;
  y = calloc( nat, sizeof(double) ) ;
  z = calloc( nat, sizeof(double) ) ;
	
// vitesses
  vx = calloc( nat, sizeof(double) ) ;
  vy = calloc( nat, sizeof(double) ) ;
  vz = calloc( nat, sizeof(double) ) ;

// deplacement
  dx = calloc( nat, sizeof(double) ) ;
  dy = calloc( nat, sizeof(double) ) ;
  dz = calloc( nat, sizeof(double) ) ;

// liste de voisins
  listevoisin = calloc( nat, sizeof(int*) ) ;
  for ( iat = 0 ; iat < nat ; iat++ ) {
    listevoisin[iat] = calloc( nbrevoisinmax, sizeof(int) ) ;
  }
  nbrevoisin = calloc( nat, sizeof(int) ) ;

// distribution radiale
  radial_dist = calloc( npas_radial_dist, sizeof(double) ) ;

  for ( ix = 0 ; ix < npas_radial_dist ; ix++ ) {
    radial_dist[ix] = 0. ;
  }

// initialisation a 0 de tous les tableaux
  for ( iat = 0 ; iat < nat ; iat++ ) {
    frcx[iat] = 0. ;
    frcy[iat] = 0. ;
    frcz[iat] = 0. ;

    x[iat] = 0. ;
    y[iat] = 0. ;
    z[iat] = 0. ;

    vx[iat] = 0. ;
    vy[iat] = 0. ;
    vz[iat] = 0. ;

    nbrevoisin[iat] = 0 ;

    for ( ivois = 0 ; ivois < nbrevoisinmax ; ivois++ ) {
      listevoisin [iat] [ivois] = 0 ;
    }
  }

// variables supplementaires pour DPDE
  if ( strcmp( methode , "DPDE" )         == 0 || 
       strcmp( methode , "DPDE_alamain" ) == 0 ||
       strcmp( methode , "DPDE_ito" )     == 0) {       
    // energie interne
    e_interne = calloc( nat, sizeof(double) ) ;

    // temperature interne
    temp_interne = calloc( nat, sizeof(double) ) ;

    for ( iat = 0 ; iat < nat ; iat++ ) {
      e_interne[iat] = 0. ;
      temp_interne[iat] = 0. ;
    }
  }

  // tirage
  wxrand = calloc( nat, sizeof(double*) ) ;
  wyrand = calloc( nat, sizeof(double*) ) ;
  wzrand = calloc( nat, sizeof(double*) ) ;
  for ( iat = 0 ; iat < nat ; iat++ ) {
    wxrand[iat] = calloc( nat, sizeof(double) ) ;
    wyrand[iat] = calloc( nat, sizeof(double) ) ;
    wzrand[iat] = calloc( nat, sizeof(double) ) ;
    for( ix = 0 ; ix < nat ; ix++ ) {
      wxrand[iat][ix] = 0. ;
      wyrand[iat][ix] = 0. ;
      wzrand[iat][ix] = 0. ;
    }
  }

  return erreur ;
}

