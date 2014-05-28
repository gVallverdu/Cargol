
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * fonctions d'impression diverses                                           * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"
#include "DPDE.h"
#include "fichier.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * impression de la configuration en angstrom a l'iteration step             *
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int print_conf ( void ) {

  int   erreur = EXIT_SUCCESS ;
  int   iat ;
  char  num[LEN], nom[LEN] ;
  FILE *mdcrd ;

  strcpy ( nom, amdcrd ) ;

  if ( ncrd > 0 ) {
    strcat( nom , ".xyz" ) ;
    mdcrd = fopen( nom , "w" ) ;
  } else if ( ncrd < 0 ) {
    sprintf( num, "_%d.xyz", numerostep ) ;
    strcat( nom, num ) ;
    mdcrd = fopen( nom, "w" ) ;
  } else {
    return erreur ;
  }

  // impression de la conf
  fprintf( mdcrd,"%d \n",nat) ;
  fprintf( mdcrd,"coordonnees xyx en angstrom, iteration = %d\n", numerostep ) ;
  for ( iat = 0 ; iat < nat ; iat++ ) {
    fprintf( mdcrd,"%s %8.3f %8.3f %8.3f\n", aname, x[iat]*1e10, y[iat]*1e10, z[iat]*1e10 );
  }

  fclose(mdcrd) ;
	
  return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * impression des vitesses a l'iteration step                                *
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int print_vel ( void ) {

  int   erreur = EXIT_SUCCESS ;
  int   iat ;
  char  num[LEN], nom[LEN] ;
  FILE *mdvel ;

  strcpy ( nom, amdvel ) ;

  if ( nvel > 0 ) {
    strcat( nom , ".xyz" ) ;
    mdvel = fopen( nom , "w" ) ;
  } else if ( nvel < 0 ) {
    sprintf( num, "_%d.xyz", numerostep ) ;
    strcat( nom, num ) ;
    mdvel = fopen( nom, "w" ) ;
  } else {
    return erreur ;
  }

  // impression des vitesses
  fprintf( mdvel,"%d \n",nat) ;
  fprintf( mdvel,"vitesse xyx en m.s-1, iteration = %d \n", numerostep ) ;
  for ( iat = 0 ; iat < nat ; iat++ ) {
    fprintf( mdvel,"%5d %8.3f %8.3f %8.3f\n", iat, vx[iat], vy[iat], vz[iat] );
  }
  fclose(mdvel) ;

  return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * impression du g(r)                                                        *
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int print_radial_dist ( void ) {

  int     erreur = EXIT_SUCCESS ;
  double  pas, val, couche, norme, dr ;
  int	  ix ;
  FILE   *radial ;

  pas = pas_radial_dist ;
  radial = fopen( aradial_dist , "w" ) ;

  // impression du g(r)
  fprintf( radial, "# distribution radiale \n") ;
  fprintf( radial, "# pas utilise : %10.4f A \n", pas*1.e10) ;

  for ( ix = 0 ; ix < npas_radial_dist-1 ; ix++ ) {

    val = (double) ix * pas + pas / 2. ;

    dr = (double) ( (ix+1)*(ix+1)*(ix+1) - ix*ix*ix ) * pas*pas*pas ;
    couche = 4. / 3. * pi * dr ;
    norme = (double) nstep * couche * (1.e6) / volume ;
    // il manque un facteur 1e6 dans norme pour 1000 atomes

    fprintf( radial, "%8.3f %14.6e \n", val*1.e10, radial_dist[ix] / norme ) ;
  }

  fclose( radial ) ;

  return erreur;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * impression de la derniere configuration pour faire un restart             * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int print_restart ( void ) {

  int   erreur = EXIT_SUCCESS ;
  int   iat ;
  FILE *frestart ;

  frestart = fopen( arestart, "w" ) ;

  // impression de la conf
  fprintf( frestart,"%d \n",nat) ;
  fprintf( frestart,"# dimension : %14.6e, %14.6e, %14.6e \n", boxx, boxy, boxz ) ;
  for ( iat = 0 ; iat < nat ; iat++ ) {
    fprintf( frestart,"%s %16.8e %16.8e %16.8e\n",aname, x[iat]*1e10, y[iat]*1e10, z[iat]*1e10 ) ;
  }
  fprintf( frestart,"# vitesse\n");
  for ( iat = 0 ; iat < nat ; iat++ ) {
    fprintf( frestart,"%s %16.8e %16.8e %16.8e\n",aname, vx[iat], vy[iat], vz[iat] ) ;
  }
  fclose(frestart) ;
	
  return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * impression au debut de la simulation                                      * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int print_data_run ( void ) {

  int     erreur = EXIT_SUCCESS ;
  double  conv, temperature ;

  etot = ektot + eptot ;
  temperature = 2. * ektot / ( 3. * (double) nat * boltzman ) ;

  fprintf ( cntrl,"\n-----------------------------------------------------------------\n" );
  fprintf ( cntrl," * Donnees du calcul : \n");
  fprintf ( cntrl,"-----------------------------------------------------------------\n" );
  fprintf ( cntrl,"methode                    : %10s   \n", methode);
  fprintf ( cntrl,"temp0                      : %12.2f K     \n", temp0) ;
  fprintf ( cntrl,"nstep                      : %10d   \n", nstep);
  fprintf ( cntrl,"dt                         : %10.3f fs\n", dt * 1.e15);
  fprintf ( cntrl,"nave                       : %10d   \n", nave);
  fprintf ( cntrl,"nout                       : %10d   \n", nout);
  fprintf ( cntrl,"ncrd                       : %10d   \n", ncrd);
  fprintf ( cntrl,"nvel                       : %10d   \n", nvel);
  if ( strstr( methode, "DPDE") != NULL ) {
    fprintf ( cntrl,"freqTint                   : %10d   \n", freqTint );
  }
  fprintf ( cntrl,"graine                     : %10d   \n", graine);

  fprintf ( cntrl,"\n-----------------------------------------------------------------\n" );
  fprintf ( cntrl," * Potentiel de paires : \n");
  fprintf ( cntrl,"-----------------------------------------------------------------\n" );
  fprintf ( cntrl,"sigma                      : %10.3f A \n", LJsigma * 1.e10 );
  fprintf ( cntrl,"epsilon                    : %10.3f eV \n", LJeps / qelectron );
  fprintf ( cntrl,"rcut                       : %10.3f A \n", rcut * 1.e10);
  fprintf ( cntrl,"rverlet                    : %10.3f A \n", rverlet * 1.e10);

  fprintf ( cntrl,"\n-----------------------------------------------------------------\n" );
  fprintf ( cntrl," * Configuration initiale : \n");
  fprintf ( cntrl,"-----------------------------------------------------------------\n" );
  if ( strcmp( restart, "non" ) == 0 ) {
    fprintf ( cntrl,"cristal                    : %10s \n", cristal );
    fprintf ( cntrl,"parametre de maille        : %8.3f A\n", a0 * 1.e10 );
    fprintf ( cntrl,"nombre de mailles          : %d,%d,%d \n", Nx, Ny, Nz );
    fprintf ( cntrl,"dimension de la boite X    : %8.3f A\n", boxx * 1.e10 );
    fprintf ( cntrl,"dimension de la boite Y    : %8.3f A\n", boxy * 1.e10);
    fprintf ( cntrl,"dimension de la boite Z    : %8.3f A\n", boxz * 1.e10);
  } else {
    fprintf ( cntrl,"lue sur le fichier         : %s \n", restart );
    fprintf ( cntrl,"dimension de la boite X    : %8.3f A\n", boxx * 1.e10 );
    fprintf ( cntrl,"dimension de la boite Y    : %8.3f A\n", boxy * 1.e10);
    fprintf ( cntrl,"dimension de la boite Z    : %8.3f A\n", boxz * 1.e10);
  }
  

  // conversion J -> kJ /mol
  conv = avogadro / ( 1000. * (double) nat ) ;

  fprintf ( cntrl,"\n-----------------------------------------------------------------\n" );
  fprintf ( cntrl," * grandeurs thermodynamiques initiales : \n");
  fprintf ( cntrl,"-----------------------------------------------------------------\n" );
  fprintf ( cntrl,"nombre de particules       : %10d         \n", nat) ;
  fprintf ( cntrl,"temperature                : %12.2f K     \n", temperature) ;
  fprintf ( cntrl,"masse totale               : %12.4e kg    \n", massetot) ;
  fprintf ( cntrl,"volume                     : %12.4e m-3   \n", volume) ;
  fprintf ( cntrl,"masse volumique            : %12.4e kg.m-3\n", mvol) ;
  fprintf ( cntrl,"energie potentielle totale : %12.4e kJ/mol", eptot * conv) ;
  fprintf ( cntrl,"        Ep / particule : %12.4e J\n", eptot / (double) nat ) ;
  fprintf ( cntrl,"energie cinetique totale   : %12.4e kJ/mol", ektot * conv) ;
  fprintf ( cntrl,"        Ec / particule : %12.4e J\n", ektot / (double) nat ) ;
  fprintf ( cntrl,"energie totale             : %12.4e kJ/mol", etot * conv) ;
  fprintf ( cntrl,"        E  / particule : %12.4e J\n", etot  / (double) nat ) ;
  fprintf ( cntrl,"vitesse du centre de masse : %12.4e ,%12.4e, %12.4e   m.s-1\n",vgx,vgy,vgz);

  // impression a l'ecran
  printf("-----------------------------------------------------------------\n" );
  printf(" * grandeurs thermodynamiques initiales : \n");
  printf("-----------------------------------------------------------------\n" );
  printf("nombre de particules       : %10d         \n", nat) ;
  printf("temperature                : %12.2f K     \n", temperature) ;
  printf("masse totale               : %12.4e kg    \n", massetot) ;
  printf("volume                     : %12.4e m-3   \n", volume) ;
  printf("masse volumique            : %12.4e kg.m-3\n", mvol) ;
  printf("energie potentielle totale : %12.4e kJ/mol", eptot * conv) ;
  printf("            Ep / particule : %12.4e J\n", eptot / (double) nat ) ;
  printf("energie cinetique totale   : %12.4e kJ/mol", ektot * conv) ;
  printf("            Ec / particule : %12.4e J\n", ektot / (double) nat ) ;
  printf("energie totale             : %12.4e kJ/mol", etot * conv) ;
  printf("            E  / particule : %12.4e J\n", etot  / (double) nat ) ;
  printf("vitesse du centre de masse : %12.4e ,%12.4e, %12.4e   m.s-1\n",vgx,vgy,vgz);

  return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * impression a la fin de la simulation                                      * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int print_fin ( int idmethode ) {

  int     erreur = EXIT_SUCCESS ;
  int     nval, iat ;
  int     h, m, s ;
  double  conv, duree ;
  char	  madate[LEN] ;
  time_t  temps ;

  end = clock() ;
  duree = ( (double) (end - t_init) ) / CLOCKS_PER_SEC ;
  h = (int) floor( duree / 3600. ) ;
  m = (int) floor( ( duree - 3600. * (double) h ) / 60. ) ;
  s = (int) ( duree - 3600. * (double) h - 60. * (double) m ) ;

  
  // nbre de valeurs utilisee pour le calcul des moyennes
  // + 1 pour step = 1
  if ( nave != 0 ) {
    nval = ( nstep - nstep % nave ) / nave + 1 ;
  } else {
    nval = 1 ;
  }

  // calcul des moyennes
  ave_etot  = ave_etot  / (double) nval ;
  ave_eptot = ave_eptot / (double) nval ;
  ave_ektot = ave_ektot / (double) nval ;
  ave_temp  = ave_temp  / (double) nval ;
  ave_pres  = ave_pres  / (double) nval ;

  sig_etot  = sig_etot  / (double) nval ;
  sig_eptot = sig_eptot / (double) nval ;
  sig_ektot = sig_ektot / (double) nval ;
  sig_temp  = sig_temp  / (double) nval ;
  sig_pres  = sig_pres  / (double) nval ;

  sig_etot  = sqrt( sig_etot  - ave_etot  * ave_etot  ) ;
  sig_ektot = sqrt( sig_ektot - ave_ektot * ave_ektot ) ;
  sig_eptot = sqrt( sig_eptot - ave_eptot * ave_eptot ) ;
  sig_temp  = sqrt( sig_temp  - ave_temp  * ave_temp  ) ;
  sig_pres  = sqrt( sig_pres  - ave_pres  * ave_pres  ) ;

  // conversions J -> kJ/mol
  conv = avogadro / ( 1000. * (double) nat ) ;
  ave_etot  = ave_etot  * conv ;
  ave_ektot = ave_ektot * conv ;
  ave_eptot = ave_eptot * conv ;
  sig_etot  = sig_etot  * conv ;
  sig_ektot = sig_ektot * conv ;
  sig_eptot = sig_eptot * conv ;

  // DPDE
  if ( idmethode == idDPDE || idmethode == idDPDE_alamain || idmethode == idDPDE_ito ) {
    ave_eitot = ave_eitot / (double) nval ;
    sig_eitot = sig_eitot / (double) nval ;
    sig_eitot = sqrt( sig_eitot - ave_eitot * ave_eitot ) ;

    ave_temp_int = ave_temp_int / (double) nval ;
    sig_temp_int = sig_temp_int / (double) nval ;
    sig_temp_int = sqrt( sig_temp_int - ave_temp_int * ave_temp_int ) ;
  }

  // impressions des moyennes et flucuations dans le fichier cntrl
  fprintf(cntrl,"\n\n-----------------------------------------------------------------\n" );
  fprintf(cntrl, " * moyennes et fluctuations apres %d pas\n", nstep );
  fprintf(cntrl,"-----------------------------------------------------------------\n" );
  fprintf(cntrl, "nombres de valeurs         : %12d \n", nval ) ;
  fprintf(cntrl, "temps total de simultaion  : %12.4e ps\n", dt * 1e12 * (double) nstep ) ;
  fprintf(cntrl, "temperature                : %12.4e +-%12.4e K\n", ave_temp, sig_temp);
  fprintf(cntrl, "pression                   : %12.4e +-%12.4e Pa\n", ave_pres, sig_pres);
  fprintf(cntrl, "energie totale             : %12.4e +-%12.4e kJ/mol\n", ave_etot, sig_etot);
  fprintf(cntrl, "energie cinetique totale   : %12.4e +-%12.4e kJ/mol\n", ave_ektot, sig_ektot);
  fprintf(cntrl, "energie potentielle totale : %12.4e +-%12.4e kJ/mol\n", ave_eptot, sig_eptot);
  if( idmethode == idDPDE || idmethode == idDPDE_alamain || idmethode == idDPDE_ito ) {
    fprintf(cntrl, "energie interne totale     : %12.4e +-%12.4e kJ/mol\n", ave_eitot,sig_eitot ) ;
    fprintf(cntrl, "temperature interne totale :") ;
    fprintf(cntrl, " %12.4e +-%12.4e K\n", ave_temp_int, sig_temp_int ) ;
  }

  temps = time(NULL) ;
  strftime ( madate, sizeof(madate), "%a %H h %M m %S s", localtime(&temps));
  fprintf(cntrl,"\n\n-----------------------------------------------------------------\n" );
  fprintf(cntrl," * fin               : %s \n", madate );
  fprintf(cntrl," * temps d'execution : %f s   (%d h %d m %d s)\n", duree, h, m , s );

  fprintf(cntrl,"-----------------------------------------------------------------\n\n" );


  // impressions des moyennes et flucuations a l'ecran
  printf("\n\n-----------------------------------------------------------------\n" );
  printf(" * moyennes et fluctuations apres %d pas\n", nstep );
  printf("-----------------------------------------------------------------\n" );
  printf("nombres de valeurs         : %12d \n", nval ) ;
  printf("temps total de simultaion  : %12.4e ps\n", dt * 1e12 * (double) nstep ) ;
  printf("temperature                : %12.4e +-%12.4e K\n", ave_temp, sig_temp);
  printf("pression                   : %12.4e +-%12.4e Pa\n", ave_pres, sig_pres);
  printf("energie totale             : %12.4e +-%12.4e kJ/mol\n", ave_etot, sig_etot);
  printf("energie cinetique totale   : %12.4e +-%12.4e kJ/mol\n", ave_ektot, sig_ektot);
  printf("energie potentielle totale : %12.4e +-%12.4e kJ/mol\n", ave_eptot, sig_eptot);
  if( idmethode == idDPDE || idmethode == idDPDE_alamain || idmethode == idDPDE_ito ) {
    printf("energie interne totale     : %12.4e +-%12.4e kJ/mol\n", ave_eitot, sig_eitot ) ;
    printf("temperature interne totale :" ) ;
    printf(" %12.4e +-%12.4e K\n", ave_temp_int, sig_temp_int ) ;
  }
  printf("\n\n-----------------------------------------------------------------\n" );
  printf(" * temps d'execution : %d h %d m %d s\n", h, m, s ) ;
  printf("-----------------------------------------------------------------\n\n" );

  // impression des dernieres temperatures et energies internes
  if ( idmethode == idDPDE || idmethode == idDPDE_alamain || idmethode == idDPDE_ito ) {
    fprintf( ftemp_int, "& derniere iteration, step = %d\n", numerostep ) ;
    for ( iat = 0 ; iat < nat ; iat++ ) {
      fprintf( ftemp_int ,"%5d ",iat ) ;
      fprintf( ftemp_int ,"%16.7e " , temp_interne[iat] ) ;
      fprintf( ftemp_int ,"%16.7e\n", e_interne[iat]*avogadro/1000. ) ;
    }
  }

  return erreur;
}

