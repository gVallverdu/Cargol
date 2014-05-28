/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           *
 * lecture ou construction de la configuration initiale                      *
 *                                                                           *
 * contient : read_configuration()	=> lecture du fichier restart        *
 *            start_configuration()	=> configuration initiale cristaline *
 *            start_vitesses()		=> tirage des vitesses initiales     *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"

extern int periodic_cond ( int , Data * , Particules ** ) ;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * lecture de la configuration initiale                                      * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int read_configuration( Data *d, Etat *e, Particules *Particule[] ) {

  FILE   *inpcrd, *frestart ;
  Particules *p  = NULL ;

  int	 erreur = EXIT_SUCCESS ;
  int	 iat ;
  char	 ligne[ LEN ] = "" ;
  double ecinet ;
  float	 tempx, tempy, tempz ;

  // ouverture du fichier restart
  frestart = fopen( d->restart, "r" ) ;

  if( fgets( ligne, LEN, frestart ) == NULL ) stop("erreur : lecture restart nombre d'atome") ;  // nat
  if( fgets( ligne, LEN, frestart ) == NULL ) stop("erreur : lecture restart taille de la boite") ;  // dimension

  // lecture positions, conversion A -> m
  for ( iat = 0 ; iat < e->nat ; iat++ ) {
    p = &((*Particule)[iat]) ;
  
    erreur += fscanf( frestart,"%*s %e %e %e\n", &tempx, &tempy, &tempz ) ;
    p->x = (double) tempx / d->Ang ;
    p->y = (double) tempy / d->Ang ;
    p->z = (double) tempz / d->Ang ;
  }

  // conditions periodiques aux limites
  periodic_cond( e->nat , d , &(*Particule) ) ;

  // lecture du flag vitesse
  if( fgets( ligne, LEN, frestart ) == NULL) printf("erreur lecture restart 3") ;

  // lecture vitesses
  e->vgx = e->vgy = e->vgz = 0. ;
  e->massetot = 0. ;
  for ( iat = 0 ; iat < e->nat ; iat++ ) {
    p = &((*Particule)[iat]) ;
  
    erreur += fscanf( frestart,"%*s %e %e %e\n", &tempx, &tempy, &tempz ) ;
    p->vx = (double) tempx ;
    p->vy = (double) tempy ;
    p->vz = (double) tempz ;

    e->massetot += d->masse ;

    // vitesse centre de gravite
    e->vgx += d->masse * p->vx ;
    e->vgy += d->masse * p->vy ;
    e->vgz += d->masse * p->vz ;
  }
  fclose( frestart ) ;

  e->vgx = e->vgx / e->massetot ;
  e->vgy = e->vgy / e->massetot ;
  e->vgz = e->vgz / e->massetot ;

  e->ektot = 0. ;
  for ( iat = 0 ; iat < e->nat ; iat++ ) {
    p = &((*Particule)[iat]) ;
    
    // ajout d'une vitesse initiale si demande
    p->vx += d->vginitx ;
    p->vy += d->vginity ;
    p->vz += d->vginitz ;

    // calcul de l'energie cinetique
    e->ektot += 0.5 * d->masse * ( p->vx * p->vx + p->vy * p->vy + p->vz * p->vz ) ;
   
  }
  ecinet = e->ektot - 0.5 * e->massetot * ( e->vgx*e->vgx + e->vgy*e->vgy + e->vgz*e->vgz ) ;
  e->temperature = 2. * ecinet / ( 3. * (double) e->nat * d->boltzman ) ;

  // impression de la configuration intiale au format xyz
  inpcrd = fopen( "inpcrd.xyz", "w" ) ;

  fprintf( inpcrd, "%d\n", e->nat ) ;
  fprintf( inpcrd, "configuration initiale lue sur le fichier %s \n", d->restart) ;
  for ( iat = 0 ; iat < e->nat ; iat++ ) {
    p = &((*Particule)[iat]) ;
  
    fprintf( inpcrd, "%s ", d->aname ) ;
    fprintf( inpcrd, "%8.3f ",   p->x * d->Ang ) ;
    fprintf( inpcrd, "%8.3f ",   p->y * d->Ang ) ;
    fprintf( inpcrd, "%8.3f \n", p->z * d->Ang ) ;
  }
  fclose( inpcrd ) ;

  return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * Construction de la configuration initiale                                 * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int start_configuration( int Natom, Data *Donnees, Particules *Particule[] ) {

  Particules *p = NULL ;
  FILE       *inpcrd = NULL ;
  
  int    erreur = EXIT_SUCCESS ;
  int    i, j, k, l, iat ;
  int    natm = 0;
  double xi[4], yi[4], zi[4] ;

  // position maille elementaire
  xi[0] = 0. ;
  yi[0] = 0. ;
  zi[0] = 0. ;

  if         ( strcmp( Donnees->cristal, "simple" ) == 0 ) {
    // maille initiale simple
    natm = 1 ;

  } else if  ( strcmp( Donnees->cristal, "centre" ) == 0 ) {
    // maille initiale centre
    natm = 2 ;
    xi[1] = 0.5 ;
    yi[1] = 0.5 ;
    zi[1] = 0.5 ;

  } else if  ( strcmp( Donnees->cristal, "CFC" ) == 0 ) {
    // maille initiale CFC
    natm = 4 ;
    xi[1] = 0.  ;
    yi[1] = 0.5 ;
    zi[1] = 0.5 ;

    xi[2] = 0.5 ;
    yi[2] = 0.  ;
    zi[2] = 0.5 ;

    xi[3] = 0.5 ;
    yi[3] = 0.5 ;
    zi[3] = 0.  ;

  }

  // construction de la configuration initiale
  iat = 0 ;
  for ( i = 0 ; i < Donnees->Nx ; i++) {
    for ( j = 0 ; j < Donnees->Ny ; j++) {
      for ( k = 0 ; k < Donnees->Nz ; k++) {
       	for ( l = 0 ; l < natm ; l++ ) {
          p = &((*Particule)[iat]) ;
      	  iat++ ;
      	  
          p->x = ( xi[l] + (double) i ) * Donnees->a0 ;
      	  p->y = ( yi[l] + (double) j ) * Donnees->a0 ;
      	  p->z = ( zi[l] + (double) k ) * Donnees->a0 ;
      	}
      }
    }
  }

  // nombre d'atome total
  if ( iat != Natom ) stop("ERREUR : calcul du nombre d'atome") ;

  // impression de la configuration intiale au format xyz
  inpcrd = fopen( "inpcrd.xyz", "w" ) ;

  fprintf( inpcrd, "%d\n", Natom ) ;
  fprintf( inpcrd, "configuration initiale en angstrom : maille %s \n", Donnees->cristal) ;
  for ( iat = 0 ; iat < Natom ; iat++ ) {
    p = &((*Particule)[iat]) ;
    
    fprintf( inpcrd, "%s ", Donnees->aname ) ;
    fprintf( inpcrd, "%8.3f ",   p->x * Donnees->Ang ); 
    fprintf( inpcrd, "%8.3f ",   p->y * Donnees->Ang ) ;
    fprintf( inpcrd, "%8.3f \n", p->z * Donnees->Ang ) ;
  }
  fclose( inpcrd ) ;

  return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * Tirages des vitesses initiales                                            * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int start_vitesse( Data *d , Etat *e, Particules *Particule[] ) {

  Particules *p = NULL ;

  int    erreur = EXIT_SUCCESS ;
  int    iat ;
  double sigma, ec ;
  double ecinet[3], scaling[3] ;

  sigma = sqrt( 2. * d->boltzman * d->temp0 / d->masse ) ;

  e->massetot = 0. ;
  e->vgx = e->vgy = e->vgz = 0. ;

  // tirage des vitesses et calcul de la vitesse du centre de gravite
  for ( iat = 0 ; iat < e->nat ; iat++ ) {
    p = &((*Particule)[iat]) ;

    p->vx = sigma * gasdev( &(d->idum) ) ;
    p->vy = sigma * gasdev( &(d->idum) ) ;
    p->vz = sigma * gasdev( &(d->idum) ) ;

    e->massetot += d->masse ;

    // vitesse centre de gravite
    e->vgx += d->masse * p->vx ;
    e->vgy += d->masse * p->vy ;
    e->vgz += d->masse * p->vz ;
  }

  e->vgx = e->vgx / e->massetot ;
  e->vgy = e->vgy / e->massetot ;
  e->vgz = e->vgz / e->massetot ;

  // calcul de ecinet et suppresion de la vitesse du centre de gravite
  ecinet[0] = ecinet[1] = ecinet[2] = 0. ;
  for ( iat = 0 ; iat < e->nat ; iat++ ) {
    p = &((*Particule)[iat]) ;
  
    p->vx -= e->vgx ;
    p->vy -= e->vgy ;
    p->vz -= e->vgz ;

    // E cinetique
    ecinet[0] += 0.5 * d->masse * p->vx * p->vx ;
    ecinet[1] += 0.5 * d->masse * p->vy * p->vy ;
    ecinet[2] += 0.5 * d->masse * p->vz * p->vz ;
  }

  // on ajuste les vitesses a la temperature temp0
  // on a nat degres de liberte par direction
  scaling[0] = sqrt( 0.5 * (double) e->nat * d->boltzman * d->temp0 / ecinet[0] ) ;
  scaling[1] = sqrt( 0.5 * (double) e->nat * d->boltzman * d->temp0 / ecinet[1] ) ;
  scaling[2] = sqrt( 0.5 * (double) e->nat * d->boltzman * d->temp0 / ecinet[2] ) ;

  ecinet[0] = ecinet[1] = ecinet[2] = 0. ;
  e->vgx = e->vgy = e->vgz = 0. ;
  for ( iat = 0 ; iat < e->nat ; iat++ ) {
    p = &((*Particule)[iat]) ;
  
    p->vx = scaling[0] * p->vx ;
    p->vy = scaling[1] * p->vy ;
    p->vz = scaling[2] * p->vz ;

    // ajout de la vitesse globale initiale
    p->vx += d->vginitx ;
    p->vy += d->vginity ;
    p->vz += d->vginitz ;
    
    // vitesse centre de gravite
    e->vgx += d->masse * p->vx ;
    e->vgy += d->masse * p->vy ;
    e->vgz += d->masse * p->vz ;

    // E cinetique
    ecinet[0] += 0.5 * d->masse * p->vx * p->vx ;
    ecinet[1] += 0.5 * d->masse * p->vy * p->vy ;
    ecinet[2] += 0.5 * d->masse * p->vz * p->vz ;
  }

  // energie cinetique totale
  e->ektot = ecinet[0] + ecinet[1] + ecinet[2] ;

  // vitesse centre de gravite
  e->vgx = e->vgx / e->massetot ;
  e->vgy = e->vgy / e->massetot ;
  e->vgz = e->vgz / e->massetot ;

  // temperature
  ec = e->ektot - 0.5 * e->massetot * ( e->vgx*e->vgx + e->vgy*e->vgy+ e->vgz*e->vgz ) ;
  e->temperature = 2. * ec / ( 3. * (double) e->nat * d->boltzman ) ; 

  return erreur ;
}

