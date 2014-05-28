
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * fonctions d'impression diverses                                           * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * impression de la configuration en angstrom a l'iteration step             *
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int print_conf( int Natom, Data *d, Particules *Particule[] ) {

  Particules *p = NULL ;
  FILE       *mdcrd = NULL ;
  
  int   erreur = EXIT_SUCCESS ;
  int   iat ;
  char  num[LEN], nom[LEN] ;

  strcpy ( nom, d->amdcrd ) ;

  if ( d->ncrd > 0 ) {
    strcat( nom , ".xyz" ) ;
    mdcrd = fopen( nom , "w" ) ;
  } else if ( d->ncrd < 0 ) {
    sprintf( num, "_%d.xyz", d->numerostep ) ;
    strcat( nom, num ) ;
    mdcrd = fopen( nom, "w" ) ;
  } else {
    return erreur ;
  }

  // impression de la conf
  fprintf( mdcrd,"%d \n", Natom) ;
  fprintf( mdcrd,"coordonnees xyx en angstrom, iteration = %d\n", d->numerostep ) ;
  for ( iat = 0 ; iat < Natom ; iat++ ) {
    p = &((*Particule)[iat]) ;
    fprintf( mdcrd,"%s ", d->aname ) ;
    fprintf( mdcrd,"%8.3f " , p->x * d->Ang ) ;
    fprintf( mdcrd,"%8.3f " , p->y * d->Ang ) ;
    fprintf( mdcrd,"%8.3f\n", p->z * d->Ang ) ;
  }

  fclose(mdcrd) ;
	
  return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * impression des vitesses a l'iteration step                                *
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int print_vel ( int Natom, Data *d, Particules *Particule[] ) {

  Particules *p = NULL ;
  FILE       *mdvel = NULL ;
  
  int   erreur =EXIT_SUCCESS ;
  int   iat ;
  char  num[LEN], nom[LEN] ;

  strcpy ( nom, d->amdvel ) ;

  if ( d->nvel > 0 ) {
    strcat( nom , ".xyz" ) ;
    mdvel = fopen( nom , "w" ) ;
  } else if ( d->nvel < 0 ) {
    sprintf( num, "_%d.xyz", d->numerostep ) ;
    strcat( nom, num ) ;
    mdvel = fopen( nom, "w" ) ;
  } else {
    return erreur ;
  }

  // impression des vitesses
  fprintf( mdvel,"%d \n", Natom ) ;
  fprintf( mdvel,"vitesse xyx en m.s-1, iteration = %d \n", d->numerostep ) ;
  for ( iat = 0 ; iat < Natom ; iat++ ) {
    p = &((*Particule)[iat]) ;
    fprintf( mdvel,"%5d ", iat ) ;
    fprintf( mdvel,"%8.3f " , p->vx ) ;
    fprintf( mdvel,"%8.3f " , p->vy ) ;
    fprintf( mdvel,"%8.3f\n", p->vz ) ;
  }
  fclose(mdvel) ;

  return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * impression du g(r)                                                        *
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int print_radial_dist ( Data *d, Etat *e ) {

  FILE   *radial = NULL ;
  int     erreur = EXIT_SUCCESS ;
  double  pas, val, couche, norme, dr ;
  int	  ix ;


  pas = d->pas_radial_dist ;
  radial = fopen( d->aradial_dist , "w" ) ;

  // impression du g(r)
  fprintf( radial, "# distribution radiale \n") ;
  fprintf( radial, "# pas utilise : %10.4f A \n", pas * d->Ang ) ;

  for ( ix = 0 ; ix < d->npas_radial_dist-1 ; ix++ ) {

    val = (double) ix * pas + pas / 2. ;

    dr = (double) ( (ix+1)*(ix+1)*(ix+1) - ix*ix*ix ) * pas*pas*pas ;
    couche = 4. / 3. * d->pi * dr ;
    norme = (double) d->nstep * couche * (1.e6) / e->volume ;
    // il manque un facteur 1e6 dans norme pour 1000 atomes

    fprintf( radial, "%8.3f %14.6e \n", val*d->Ang, d->radial_dist[ix] / norme ) ;
  }

  fclose( radial ) ;

  return erreur;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * impression de la derniere configuration pour faire un restart             * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int print_restart ( int Natom, Data *d, Particules *Particule[] ) {

  Particules  *p = NULL ;
  FILE        *frestart = NULL ;
  
  int  erreur = EXIT_SUCCESS ;
  int  iat ;

  frestart = fopen( d->arestart, "w" ) ;

  // impression de la conf
  fprintf( frestart,"%d \n", Natom) ;
  fprintf( frestart,"# dimension : ") ;
  fprintf( frestart,"%14.6e, ", d->boxx ) ;
  fprintf( frestart,"%14.6e, ", d->boxy ) ;
  fprintf( frestart,"%14.6e\n", d->boxz ) ;
  for ( iat = 0 ; iat < Natom ; iat++ ) {
    p = &((*Particule)[iat]) ;
    fprintf( frestart,"%s ", d->aname ) ;
    fprintf( frestart,"%16.8e " , p->x * d->Ang ) ;
    fprintf( frestart,"%16.8e " , p->y * d->Ang ) ;
    fprintf( frestart,"%16.8e\n", p->z * d->Ang ) ;
  }
  
  fprintf( frestart,"# vitesse\n");
  for ( iat = 0 ; iat < Natom ; iat++ ) {
    p = &((*Particule)[iat]) ;
    fprintf( frestart,"%s ", d->aname ) ;
    fprintf( frestart,"%16.8e " , p->vx ) ;
    fprintf( frestart,"%16.8e " , p->vy ) ;
    fprintf( frestart,"%16.8e\n", p->vz ) ;
  }
  fclose(frestart) ;
	
  return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * impression au debut de la simulation                                      * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int print_data_run ( Data *d, Etat *e ) {

  FILE    *cntrl = d->cntrl ;
  int     erreur = EXIT_SUCCESS ;
  double  conv ;

  e->etot = e->ektot + e->eptot ;

  fprintf ( cntrl,"\n-----------------------------------------------------------------\n" );
  fprintf ( cntrl," * Donnees du calcul : \n");
  fprintf ( cntrl,"-----------------------------------------------------------------\n" );
  fprintf ( cntrl,"methode                    : %10s   \n", d->methode);
  fprintf ( cntrl,"temp0                      : %12.2f K \n", d->temp0) ;
  fprintf ( cntrl,"nstep                      : %10d   \n", d->nstep);
  fprintf ( cntrl,"dt                         : %10.3f fs\n", d->dt * 1.e15);
  fprintf ( cntrl,"nave                       : %10d   \n", d->nave);
  fprintf ( cntrl,"nout                       : %10d   \n", d->nout);
  fprintf ( cntrl,"ncrd                       : %10d   \n", d->ncrd);
  fprintf ( cntrl,"nvel                       : %10d   \n", d->nvel);
  if ( strstr( d->methode, "DPDE") != NULL ) {
    fprintf ( cntrl,"freqTint                   : %10d   \n", d->freqTint );
  }
  fprintf ( cntrl,"graine                     : %10d   \n", d->graine);

  fprintf ( cntrl,"\n-----------------------------------------------------------------\n" );
  fprintf ( cntrl," * Potentiel de paires : \n");
  fprintf ( cntrl,"-----------------------------------------------------------------\n" );
  fprintf ( cntrl,"sigma                      : %10.3f A \n", d->LJsigma * 1.e10 );
  fprintf ( cntrl,"epsilon                    : %10.3f eV \n", d->LJeps / d->qelectron );
  fprintf ( cntrl,"rcut                       : %10.3f A \n", d->rcut * 1.e10);
  fprintf ( cntrl,"rverlet                    : %10.3f A \n", d->rverlet * 1.e10);

  fprintf ( cntrl,"\n-----------------------------------------------------------------\n" );
  fprintf ( cntrl," * Configuration initiale : \n");
  fprintf ( cntrl,"-----------------------------------------------------------------\n" );
  if ( strcmp( d->restart, "non" ) == 0 ) {
    fprintf ( cntrl,"cristal                    : %10s \n", d->cristal );
    fprintf ( cntrl,"parametre de maille        : %8.3f A\n", d->a0 * 1.e10 );
    fprintf ( cntrl,"nombre de mailles          : %d,%d,%d \n", d->Nx, d->Ny, d->Nz );
    fprintf ( cntrl,"dimension de la boite X    : %8.3f A\n", d->boxx * 1.e10 );
    fprintf ( cntrl,"dimension de la boite Y    : %8.3f A\n", d->boxy * 1.e10);
    fprintf ( cntrl,"dimension de la boite Z    : %8.3f A\n", d->boxz * 1.e10);
  } else {
    fprintf ( cntrl,"lue sur le fichier         : %s \n", d->restart );
    fprintf ( cntrl,"dimension de la boite X    : %8.3f A\n", d->boxx * 1.e10 );
    fprintf ( cntrl,"dimension de la boite Y    : %8.3f A\n", d->boxy * 1.e10);
    fprintf ( cntrl,"dimension de la boite Z    : %8.3f A\n", d->boxz * 1.e10);
  }
  

  // conversion J -> kJ /mol
  conv = d->avogadro / ( 1000. * (double) e->nat ) ;

  fprintf ( cntrl,"\n-----------------------------------------------------------------\n" );
  fprintf ( cntrl," * grandeurs thermodynamiques initiales : \n");
  fprintf ( cntrl,"-----------------------------------------------------------------\n" );
  fprintf ( cntrl,"nombre de particules       : %10d         \n", e->nat) ;
  fprintf ( cntrl,"temperature                : %12.2f K     \n", e->temperature) ;
  fprintf ( cntrl,"masse totale               : %12.4e kg    \n", e->massetot) ;
  fprintf ( cntrl,"volume                     : %12.4e m-3   \n", e->volume) ;
  fprintf ( cntrl,"masse volumique            : %12.4e kg.m-3\n", e->mvol) ;
  fprintf ( cntrl,"energie potentielle totale : %12.4e kJ/mol", e->eptot * conv) ;
  fprintf ( cntrl,"        Ep / particule : %12.4e J\n", e->eptot / (double) e->nat ) ;
  fprintf ( cntrl,"energie cinetique totale   : %12.4e kJ/mol", e->ektot * conv) ;
  fprintf ( cntrl,"        Ec / particule : %12.4e J\n", e->ektot / (double) e->nat ) ;
  fprintf ( cntrl,"energie totale             : %12.4e kJ/mol", e->etot * conv) ;
  fprintf ( cntrl,"        E  / particule : %12.4e J\n", e->etot  / (double) e->nat ) ;
  fprintf ( cntrl,"vitesse du centre de masse : %12.4e ,%12.4e, %12.4e   m.s-1\n",e->vgx,e->vgy,e->vgz);

  // impression a l'ecran
  printf("-----------------------------------------------------------------\n" );
  printf(" * grandeurs thermodynamiques initiales : \n");
  printf("-----------------------------------------------------------------\n" );
  printf("nombre de particules       : %10d         \n", e->nat) ;
  printf("temperature                : %12.2f K     \n", e->temperature) ;
  printf("masse totale               : %12.4e kg    \n", e->massetot) ;
  printf("volume                     : %12.4e m-3   \n", e->volume) ;
  printf("masse volumique            : %12.4e kg.m-3\n", e->mvol) ;
  printf("energie potentielle totale : %12.4e kJ/mol", e->eptot * conv) ;
  printf("            Ep / particule : %12.4e J\n", e->eptot / (double) e->nat ) ;
  printf("energie cinetique totale   : %12.4e kJ/mol", e->ektot * conv) ;
  printf("            Ec / particule : %12.4e J\n", e->ektot / (double) e->nat ) ;
  printf("energie totale             : %12.4e kJ/mol", e->etot * conv) ;
  printf("            E  / particule : %12.4e J\n", e->etot  / (double) e->nat ) ;
  printf("vitesse du centre de masse : %12.4e ,%12.4e, %12.4e   m.s-1\n",e->vgx,e->vgy,e->vgz);

  return erreur ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * impression a la fin de la simulation                                      * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int print_fin ( int idmethode, Data *d, Etat *e, Particules *Particule[] ) {

  Particules *p = NULL ;
  time_t     temps ;
  FILE       *cntrl = d->cntrl ;
  
  int     erreur = EXIT_SUCCESS ;
  int     nval, iat ;
  int     h, m, s ;
  double  conv, duree ;
  char	  madate[LEN] ;

  d->end = clock() ;
  duree = ( (double) (d->end - d->t_init) ) / CLOCKS_PER_SEC ;
  h = (int) floor( duree / 3600. ) ;
  m = (int) floor( ( duree - 3600. * (double) h ) / 60. ) ;
  s = (int) ( duree - 3600. * (double) h - 60. * (double) m ) ;

  
  // nbre de valeurs utilisee pour le calcul des moyennes
  // + 1 pour step = 1
  if ( d->nave != 0 ) {
    nval = ( d->nstep - d->nstep % d->nave ) / d->nave + 1 ;
  } else {
    nval = 1 ;
  }

  // calcul des moyennes
  e->ave_etot  = e->ave_etot  / (double) nval ;
  e->ave_eptot = e->ave_eptot / (double) nval ;
  e->ave_ektot = e->ave_ektot / (double) nval ;
  e->ave_temp  = e->ave_temp  / (double) nval ;
  e->ave_pres  = e->ave_pres  / (double) nval ;

  e->sig_etot  = e->sig_etot  / (double) nval ;
  e->sig_eptot = e->sig_eptot / (double) nval ;
  e->sig_ektot = e->sig_ektot / (double) nval ;
  e->sig_temp  = e->sig_temp  / (double) nval ;
  e->sig_pres  = e->sig_pres  / (double) nval ;

  e->sig_etot  = sqrt( e->sig_etot  - e->ave_etot  * e->ave_etot  ) ;
  e->sig_ektot = sqrt( e->sig_ektot - e->ave_ektot * e->ave_ektot ) ;
  e->sig_eptot = sqrt( e->sig_eptot - e->ave_eptot * e->ave_eptot ) ;
  e->sig_temp  = sqrt( e->sig_temp  - e->ave_temp  * e->ave_temp  ) ;
  e->sig_pres  = sqrt( e->sig_pres  - e->ave_pres  * e->ave_pres  ) ;

  // conversions J -> kJ/mol
  conv = d->avogadro / ( 1000. * (double) e->nat ) ;
  e->ave_etot  = e->ave_etot  * conv ;
  e->ave_ektot = e->ave_ektot * conv ;
  e->ave_eptot = e->ave_eptot * conv ;
  e->sig_etot  = e->sig_etot  * conv ;
  e->sig_ektot = e->sig_ektot * conv ;
  e->sig_eptot = e->sig_eptot * conv ;

  // DPDE
  if ( idmethode == idDPDE || idmethode == idDPDE_alamain || idmethode == idDPDE_ito ) {
    e->ave_eitot = e->ave_eitot / (double) nval ;
    e->sig_eitot = e->sig_eitot / (double) nval ;
    e->sig_eitot = sqrt( e->sig_eitot - e->ave_eitot * e->ave_eitot ) ;

    e->ave_temp_int = e->ave_temp_int / (double) nval ;
    e->sig_temp_int = e->sig_temp_int / (double) nval ;
    e->sig_temp_int = sqrt( e->sig_temp_int - e->ave_temp_int * e->ave_temp_int ) ;
  }

  // impressions des moyennes et flucuations dans le fichier cntrl
  fprintf(cntrl,"\n\n-----------------------------------------------------------------\n" );
  fprintf(cntrl, " * moyennes et fluctuations apres %d pas\n", d->nstep );
  fprintf(cntrl,"-----------------------------------------------------------------\n" );
  fprintf(cntrl, "nombres de valeurs         : %12d \n", nval ) ;
  fprintf(cntrl, "temps total de simultaion  : %12.4e ps\n", d->dt * 1.e12 * (double) d->nstep ) ;
  fprintf(cntrl, "temperature                : %12.4e +-%12.4e K\n", e->ave_temp, e->sig_temp);
  fprintf(cntrl, "pression                   : %12.4e +-%12.4e Pa\n", e->ave_pres, e->sig_pres);
  fprintf(cntrl, "energie totale             : %12.4e +-%12.4e kJ/mol\n", e->ave_etot, e->sig_etot);
  fprintf(cntrl, "energie cinetique totale   : %12.4e +-%12.4e kJ/mol\n", e->ave_ektot, e->sig_ektot);
  fprintf(cntrl, "energie potentielle totale : %12.4e +-%12.4e kJ/mol\n", e->ave_eptot, e->sig_eptot);
  if( idmethode == idDPDE || idmethode == idDPDE_alamain || idmethode == idDPDE_ito ) {
    fprintf(cntrl, "energie interne totale     : %12.4e +-%12.4e kJ/mol\n", e->ave_eitot, e->sig_eitot ) ;
    fprintf(cntrl, "temperature interne totale :") ;
    fprintf(cntrl, " %12.4e +-%12.4e kJ/mol\n", e->ave_temp_int, e->sig_temp_int ) ;
  }

  temps = time(NULL) ;
  strftime ( madate, sizeof(madate), "%a %H h %M m %S s", localtime(&temps));
  fprintf(cntrl,"\n\n-----------------------------------------------------------------\n" );
  fprintf(cntrl," * fin               : %s \n", madate );
  fprintf(cntrl," * temps d'execution : %f s   (%d h %d m %d s)\n", duree, h, m , s );

  fprintf(cntrl,"-----------------------------------------------------------------\n\n" );


  // impressions des moyennes et flucuations a l'ecran
  printf("\n\n-----------------------------------------------------------------\n" );
  printf(" * moyennes et fluctuations apres %d pas\n", d->nstep );
  printf("-----------------------------------------------------------------\n" );
  printf("nombres de valeurs         : %12d \n", nval ) ;
  printf("temps total de simultaion  : %12.4e ps\n", d->dt * 1.e12 * (double) d->nstep ) ;
  printf("temperature                : %12.4e +-%12.4e K\n", e->ave_temp, e->sig_temp);
  printf("pression                   : %12.4e +-%12.4e Pa\n", e->ave_pres, e->sig_pres);
  printf("energie totale             : %12.4e +-%12.4e kJ/mol\n", e->ave_etot, e->sig_etot);
  printf("energie cinetique totale   : %12.4e +-%12.4e kJ/mol\n", e->ave_ektot, e->sig_ektot);
  printf("energie potentielle totale : %12.4e +-%12.4e kJ/mol\n", e->ave_eptot, e->sig_eptot);
  if( idmethode == idDPDE || idmethode == idDPDE_alamain || idmethode == idDPDE_ito ) {
    printf("energie interne totale     : %12.4e +-%12.4e kJ/mol\n", e->ave_eitot, e->sig_eitot ) ;
    printf("temperature interne totale :" ) ;
    printf(" %12.4e +-%12.4e kJ/mol\n", e->ave_temp_int, e->sig_temp_int ) ;
  }
  printf("\n\n-----------------------------------------------------------------\n" );
  printf(" * temps d'execution : %d h %d m %d s\n", h, m, s ) ;
  printf("-----------------------------------------------------------------\n\n" );

  // impression des dernieres temperatures et energies internes
  if ( idmethode == idDPDE || idmethode == idDPDE_alamain || idmethode == idDPDE_ito ) {
    fprintf( d->ftemp_int, "& derniere iteration, step = %d\n", d->numerostep ) ;
    for ( iat = 0 ; iat < e->nat ; iat++ ) {
      p = &((*Particule)[iat]) ;
      fprintf( d->ftemp_int ,"%5d ",iat ) ;
      fprintf( d->ftemp_int ,"%16.7e " , p->Tint ) ;
      fprintf( d->ftemp_int ,"%16.7e\n", p->Eint * d->avogadro/1000. ) ;
    }
  }

  return erreur;
}

