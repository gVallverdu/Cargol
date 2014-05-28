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
#include "extern.h"

extern int periodic_cond ( void ) ;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * lecture de la configuration initiale                                      * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int read_configuration( void ) {

  int	 erreur = EXIT_SUCCESS ;
  int	 i ;
  char	 ligne[ LEN ] = "" ;
  float	 tempx, tempy, tempz ;
  double ecinet ;
  FILE   *inpcrd, *frestart ;

  // ouverture du fichier restart
  frestart = fopen( restart, "r" ) ;

  if( fgets( ligne, LEN, frestart ) == NULL ) stop("erreur : lecture restart nombre d'atome") ;  // nat
  if( fgets( ligne, LEN, frestart ) == NULL ) stop("erreur : lecture restart taille de la boite") ;  // dimension

  // lecture positions, conversion A -> m
  for ( i = 0 ; i < nat ; i++ ) {
    erreur += fscanf( frestart,"%*s %e %e %e\n", &tempx, &tempy, &tempz ) ;
    x[i] = (double) tempx * 1.e-10 ;
    y[i] = (double) tempy * 1.e-10 ;
    z[i] = (double) tempz * 1.e-10 ;
  }

  // conditions periodiques aux limites
  periodic_cond() ;

  // lecture du flag vitesse
  if( fgets( ligne, LEN, frestart ) == NULL) printf("erreur lecture restart 3") ;

  // lecture vitesses
  vgx = vgy = vgz = 0. ;
  massetot = 0. ;
  for ( i = 0 ; i < nat ; i++ ) {
    erreur += fscanf( frestart,"%*s %e %e %e\n", &tempx, &tempy, &tempz ) ;
    vx[i] = (double) tempx ;
    vy[i] = (double) tempy ;
    vz[i] = (double) tempz ;

    massetot += masse ;

    // vitesse centre de gravite
    vgx += masse * vx[i] ;
    vgy += masse * vy[i] ;
    vgz += masse * vz[i] ;
  }
  fclose( frestart ) ;

  vgx = vgx / massetot ;
  vgy = vgy / massetot ;
  vgz = vgz / massetot ;

  ektot = 0. ;
  for ( i = 0 ; i < nat ; i++ ) {

    // ajout d'une vitesse initiale si demande
    vx[i] += vginitx ;
    vy[i] += vginity ;
    vz[i] += vginitz ;

    // calcul de l'energie cinetique
    ektot += 0.5 * masse * ( vx[i]*vx[i] + vy[i]*vy[i] + vz[i]*vz[i] ) ;
  }
  ecinet = ektot - 0.5 * massetot * ( vgx*vgx + vgy*vgy + vgz*vgz ) ;
  temperature = 2. * ecinet / ( 3. * (double) nat * boltzman ) ;

  // impression de la configuration intiale au format xyz
  inpcrd = fopen( "inpcrd.xyz", "w" ) ;

  fprintf( inpcrd, "%d\n", nat ) ;
  fprintf( inpcrd, "configuration initiale lue sur le fichier %s \n", restart) ;
  for ( i = 0 ; i < nat ; i++ ) {
    fprintf( inpcrd, "%s %8.3f %8.3f %8.3f \n", 
    aname, x[i]*1e10, y[i]*1e10, z[i]*1e10 ) ;
  }
  fclose( inpcrd ) ;

  return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * Construction de la configuration initiale                                 * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int start_configuration( void ) {

  int    erreur = EXIT_SUCCESS ;
  int    i, j, k, l, iat ;
  int    natm = 0;
  double xi[4], yi[4], zi[4] ;
  FILE   *inpcrd ;

  // position maille elementaire
  xi[0] = 0. ;
  yi[0] = 0. ;
  zi[0] = 0. ;

  if         ( strcmp( cristal, "simple" ) == 0 ) {
    // maille initiale simple
    natm = 1 ;

  } else if  ( strcmp( cristal, "centre" ) == 0 ) {
    // maille initiale centre
    natm = 2 ;
    xi[1] = 0.5 ;
    yi[1] = 0.5 ;
    zi[1] = 0.5 ;

  } else if  ( strcmp( cristal, "CFC" ) == 0 ) {
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
  for ( i = 0 ; i < Nx ; i++) {
    for ( j = 0 ; j < Ny ; j++) {
      for ( k = 0 ; k < Nz ; k++) {
       	for ( l = 0 ; l < natm ; l++ ) {

          x[iat] = ( xi[l] + (double) i ) * a0 ;
      	  y[iat] = ( yi[l] + (double) j ) * a0 ;
      	  z[iat] = ( zi[l] + (double) k ) * a0 ;

      	  iat++ ;
      	}
      }
    }
  }

  // nombre d'atome total
  if ( iat != nat ) stop("ERREUR : calcul du nombre d'atome") ;

  // impression de la configuration intiale au format xyz
  inpcrd = fopen( "inpcrd.xyz", "w" ) ;

  fprintf( inpcrd, "%d\n", nat ) ;
  fprintf( inpcrd, "configuration initiale en angstrom : maille %s \n", cristal) ;
  for ( iat = 0 ; iat < nat ; iat++ ) {
    fprintf( inpcrd, "%s %8.3f %8.3f %8.3f \n", 
    aname, x[iat]*1e10, y[iat]*1e10, z[iat]*1e10 ) ;
  }
  fclose( inpcrd ) ;

  return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * Tirages des vitesses initiales                                            * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int start_vitesse( void ) {

  int    erreur = EXIT_SUCCESS ;
  int    iat ;
  double sigma, ec ;
  double ecinet[3], scaling[3] ;

  sigma = sqrt( 2. * boltzman * temp0 / masse ) ;

  massetot = 0. ;
  vgx = vgy = vgz = 0. ;

  // tirage des vitesses et calcul de la vitesse du centre de gravite
  for ( iat = 0 ; iat < nat ; iat++ ) {

    vx[iat] = sigma * gasdev( &idum ) ;
    vy[iat] = sigma * gasdev( &idum ) ;
    vz[iat] = sigma * gasdev( &idum ) ;

    massetot += masse ;

    // vitesse centre de gravite
    vgx += masse * vx[iat] ;
    vgy += masse * vy[iat] ;
    vgz += masse * vz[iat] ;
  }

  vgx = vgx / massetot ;
  vgy = vgy / massetot ;
  vgz = vgz / massetot ;

  // calcul de ecinet et suppresion de la vitesse du centre de gravite
  ecinet[0] = ecinet[1] = ecinet[2] = 0. ;
  for ( iat = 0 ; iat < nat ; iat++ ) {
    vx[iat] = vx[iat] - vgx ;
    vy[iat] = vy[iat] - vgy ;
    vz[iat] = vz[iat] - vgz ;

    // E cinetique
    ecinet[0] += 0.5 * masse * vx[iat] * vx[iat] ;
    ecinet[1] += 0.5 * masse * vy[iat] * vy[iat] ;
    ecinet[2] += 0.5 * masse * vz[iat] * vz[iat] ;
  }

  // on ajuste les vitesses a la temperature temp0
  // on a nat degres de liberte par direction
  scaling[0] = sqrt( 0.5 * (double) nat * boltzman * temp0 / ecinet[0] ) ;
  scaling[1] = sqrt( 0.5 * (double) nat * boltzman * temp0 / ecinet[1] ) ;
  scaling[2] = sqrt( 0.5 * (double) nat * boltzman * temp0 / ecinet[2] ) ;

  ecinet[0] = ecinet[1] = ecinet[2] = 0. ;
  vgx = vgy = vgz = 0. ;
  for ( iat = 0 ; iat < nat ; iat++ ) {
    vx[iat] = scaling[0] * vx[iat] ;
    vy[iat] = scaling[1] * vy[iat] ;
    vz[iat] = scaling[2] * vz[iat] ;

    // ajout de la vitesse globale initiale
    vx[iat] += vginitx ;
    vy[iat] += vginity ;
    vz[iat] += vginitz ;
    
    // vitesse centre de gravite
    vgx += masse * vx[iat] ;
    vgy += masse * vy[iat] ;
    vgz += masse * vz[iat] ;

    // E cinetique
    ecinet[0] += 0.5 * masse * vx[iat] * vx[iat] ;
    ecinet[1] += 0.5 * masse * vy[iat] * vy[iat] ;
    ecinet[2] += 0.5 * masse * vz[iat] * vz[iat] ;
  }

  vgx = vgx / massetot ;
  vgy = vgy / massetot ;
  vgz = vgz / massetot ;

  ektot = ecinet[0] + ecinet[1] + ecinet[2] ;

  ec = ektot - 0.5 * massetot * ( vgx*vgx + vgy*vgy + vgz*vgz ) ;
  temperature = 2. * ec / ( 3. * (double) nat * boltzman ) ;

  return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

