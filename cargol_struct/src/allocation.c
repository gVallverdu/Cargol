/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * allocation des tableaux dynamique : forces, positions, vitesses           * 
 *                                                                           * 
 * initialistion a 0 de tous les tableaux                                    * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"

// extern double **wxrand, **wyrand, **wzrand ;

int allocation( int Natom, Data *Donnees, Particules *Particule[]  ) {

  Particules *p = NULL ;
  int erreur = EXIT_SUCCESS ;
  int iat, ivois, ix ;
  
// structure particule
  *Particule = calloc( Natom , sizeof(Particules) ) ;
  for ( iat = 0 ; iat < Natom ; iat++ ) {
    (*Particule)[iat].listevoisin = calloc( Donnees->nbrevoisinmax, sizeof(int) ) ;
  }
    
// distribution radiale
  Donnees->radial_dist = calloc( Donnees->npas_radial_dist, sizeof(double) ) ;
  for( ix = 0 ; ix < Donnees->npas_radial_dist ; ix++ ) Donnees->radial_dist[ix] = 0. ;

// initialisation a 0 de tous les tableaux
  for ( iat = 0 ; iat < Natom ; iat++ ) {
    p = &((*Particule)[iat]) ;
   
    p->x = 0. ;
    p->y = 0. ;
    p->z = 0. ;

    p->vx = 0. ;
    p->vy = 0. ;
    p->vz = 0. ;
    
    p->frcx = 0. ;
    p->frcy = 0. ;
    p->frcz = 0. ;

    p->dx = 0. ;
    p->dy = 0. ;
    p->dz = 0. ;

    p->nbrevoisin = 0 ;

    for( ivois = 0 ; ivois < Donnees->nbrevoisinmax ; ivois++ )
    	p->listevoisin[ivois] = 0 ;

// variables supplementaires pour DPDE
    if ( strcmp( Donnees->methode , "DPDE" )         == 0 || 
      strcmp( Donnees->methode , "DPDE_alamain" ) == 0 ||
      strcmp( Donnees->methode , "DPDE_ito" )     == 0) {
      for ( iat = 0 ; iat < Natom ; iat++ ) {
        p->Eint = 0. ;
        p->Tint = 0. ;
      }
    }
  }

  return erreur ;
}

