
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * lecture du fichier input                                                  * 
 *                                                                           * 
 * test des valeurs lues                                                     * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"

int lecture_input( Data *Donnees, Etat *EtatThermo ) {

  Namelist maliste[NFLAG] ;

  int    erreur = EXIT_SUCCESS ;
  int    i, l, existe ;
  char	 ligne[ LEN ] = "" ;
  float	 tempx, tempy, tempz ;
  double r6 ;
  FILE  *frestart ;
  FILE  *finput ;

  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   *                                                       *
   *            initialisation de la namelist              *
   *                                                       *
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

  // liste des flags de la nameliste
  // parametres specifique au calcul
  strcpy( maliste[ 0].flag , "methode" ) ;
  strcpy( maliste[ 1].flag , "nstep" ) ;
  strcpy( maliste[ 2].flag , "dt" ) ;
  strcpy( maliste[ 3].flag , "xsi" ) ;
  strcpy( maliste[ 4].flag , "temp0" ) ;
  strcpy( maliste[ 5].flag , "rcut" ) ;
  strcpy( maliste[ 6].flag , "rverlet" ) ; 
  // nombre aleatoires
  strcpy( maliste[ 7].flag , "graine" ) ;
  // parametre du potentiel et des atomes
  strcpy( maliste[ 8].flag , "aname" ) ;
  strcpy( maliste[ 9].flag , "masse" ) ;
  strcpy( maliste[10].flag , "LJsigma" ) ;
  strcpy( maliste[11].flag , "LJeps" ) ;
  // parametres geometries initiales
  strcpy( maliste[12].flag , "cristal" ) ;
  strcpy( maliste[13].flag , "a0" ) ;
  strcpy( maliste[14].flag , "dimension" ) ;
  // voisin
  strcpy( maliste[15].flag , "nbrevoisinmax" ) ;
  // frequence output
  strcpy( maliste[16].flag , "nave" ) ;
  strcpy( maliste[17].flag , "ncrd" ) ;
  strcpy( maliste[18].flag , "nout" ) ;
  strcpy( maliste[19].flag , "nvel" ) ;
  // pas en R pour le calcul de g(r)
  strcpy( maliste[20].flag , "pas_radial_dist" ) ;
  // restart
  strcpy( maliste[21].flag , "restart" ) ;
  // vitesse globale initiale
  strcpy( maliste[22].flag , "vg_initiale" ) ;
  // pour la DPDE
  strcpy( maliste[23].flag , "capacite_calo" ) ;
  strcpy( maliste[24].flag , "temp_interne0" ) ;
  strcpy( maliste[25].flag , "freqTint" ) ;  
  strcpy( maliste[26].flag , "sigmaDPDE" ) ;  
  
  // pour savoir si le flag a ete lu ou pas
  for ( i = 0 ; i < NFLAG ; i++ ) maliste[i].lu = 0 ; 


  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   *                                                       *
   *             lecture du fichier d'input                *
   *                                                       *
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

  // fichier contenant la namelist
  finput = fopen ( Donnees->input , "r" ) ;

  // lecture du fichier input qui contient la namelist
  l = 0 ;
  while ( fgets( ligne, LEN, finput ) != NULL ) {

    l++ ;

    // test ligne blanche
    i = 0 ;
    while ( ligne[i] != '\n' && ( ligne[i] == ' ' || ligne[i] == '\t' ) ) ++i ;
    if ( ligne[i] == '\n' ) continue ;
 
    // si c'est un commentaire
    if ( strchr( &ligne[0], '#' ) != NULL || 
         strchr( &ligne[0], '*' ) != NULL ) continue ; 

    // identification du flag et lecture de la valeur
    existe = 0 ;
    for ( i = 0 ; i < NFLAG ; i++ ) {

      if ( strstr( ligne, maliste[i].flag ) != NULL ) {

        // on a identifie un flag 
        if ( maliste[i].lu == 1 ) {
          printf("\n\nflag %s, ", maliste[i].flag ) ;
          printf("sur la ligne %d :  ## ", l ) ;
          i = 0 ;
          while ( ligne[i] != '\n' ) {
            printf("%c", ligne[i] ) ;
            i++ ;
          }
          printf(" ##\n") ;
          stop( "erreur : un flag apparait plusieurs fois " );

        } else {
	  // lecture du flag
          maliste[i].lu = 1 ;
          existe = 1 ;
          sscanf ( ligne , "%*s %s", maliste[i].valeur ) ;
          break ;
        }
      }
    }

    if ( existe == 0 ) {
      printf("\n\nErreur lors de la lecture du fichier input : %s\n", Donnees->input ) ;
      printf("sur la ligne %d :  ## ", l ) ;
      i = 0 ;
      while ( ligne[i] != '\n' ) {
        printf("%c", ligne[i] ) ;
        i++ ;
      }
      printf(" ##\n") ;
      stop("erreur : flag inconnu !") ;
    }
  }

  fclose( finput ) ;

/*  for ( i = 0 ; i < NFLAG ; i++ ) {
    printf("flag : %s \t valeur : %s \n", maliste[i].flag, maliste[i].valeur );
  }
  */
  

  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   *                                                         *
   * si un flag a ete lu on attribue la valeur a la variable *
   * qui lui correspond dans variables_globales.h            *
   *                                                         *
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

  if ( maliste[0].lu == 1 ) {
    strcpy( Donnees->methode, maliste[0].valeur ) ;
    if ( strcmp( Donnees->methode, "NVE" )          != 0 &&
         strcmp( Donnees->methode, "langevin" )     != 0 &&
         strcmp( Donnees->methode, "LNVT" )         != 0 &&
         strcmp( Donnees->methode, "DPD" )          != 0 &&
         strcmp( Donnees->methode, "DPDnosplit" )   != 0 &&
         strcmp( Donnees->methode, "DPDE_alamain" ) != 0 &&
         strcmp( Donnees->methode, "DPDE_ito" )     != 0 &&
         strcmp( Donnees->methode, "DPDE" )         != 0 ) 
    {
      stop(" erreur flag methode : type de calcul inconnu" ) ;
    }
  }

  if ( maliste[ 1].lu == 1 ) Donnees->nstep   = atoi ( maliste[ 1].valeur ) ;
  if ( maliste[ 2].lu == 1 ) Donnees->dt      = atof ( maliste[ 2].valeur ) ;
  if ( maliste[ 3].lu == 1 ) Donnees->xsi     = atof ( maliste[ 3].valeur ) ;
  if ( maliste[ 4].lu == 1 ) Donnees->temp0   = atof ( maliste[ 4].valeur ) ;
  if ( maliste[ 5].lu == 1 ) Donnees->rcut    = atof ( maliste[ 5].valeur ) ;
  if ( maliste[ 6].lu == 1 ) Donnees->rverlet = atof ( maliste[ 6].valeur ) ;
  if ( maliste[ 7].lu == 1 ) Donnees->graine  = atoi ( maliste[ 7].valeur ) ;
  if ( maliste[ 8].lu == 1 ) strcpy ( Donnees->aname , maliste[ 8].valeur ) ;
  if ( maliste[ 9].lu == 1 ) Donnees->masse   = atof ( maliste[ 9].valeur ) ;
  if ( maliste[10].lu == 1 ) Donnees->LJsigma = atof ( maliste[10].valeur ) ;
  if ( maliste[11].lu == 1 ) Donnees->LJeps   = atof ( maliste[11].valeur ) ;

  if ( maliste[12].lu == 1 ) {
    strcpy( Donnees->cristal, maliste[12].valeur ) ;
    if ( strcmp( Donnees->cristal, "simple" ) != 0 && 
         strcmp( Donnees->cristal, "centre" ) != 0 &&
         strcmp( Donnees->cristal, "CFC" )    != 0 ) {
      stop("erreur flag cristal : type de maille elementaire inconnu") ;
    }
  }

  if ( maliste[13].lu == 1 ) Donnees->a0 = atof ( maliste[13].valeur ) ;

  if ( maliste[14].lu == 1 ) {
    sscanf ( maliste[14].valeur , "%d , %d , %d", &Donnees->Nx, &Donnees->Ny, &Donnees->Nz ) ;
    if ( Donnees->Nx == 0 || Donnees->Ny == 0 || Donnees->Nz == 0 ) {
      stop("erreur dimension : les dimensions doivent etre > ou = a 1\n\n pas d'espace entre les dimensions : 10,10,10" ) ;
    }
  }

  if ( maliste[15].lu == 1 ) Donnees->nbrevoisinmax = atoi ( maliste[15].valeur ) ;
  if ( maliste[16].lu == 1 ) Donnees->nave = atoi ( maliste[16].valeur ) ;
  if ( maliste[17].lu == 1 ) Donnees->ncrd = atoi ( maliste[17].valeur ) ;
  if ( maliste[18].lu == 1 ) Donnees->nout = atoi ( maliste[18].valeur ) ;
  if ( maliste[19].lu == 1 ) Donnees->nvel = atoi ( maliste[19].valeur ) ;
  if ( maliste[20].lu == 1 ) Donnees->pas_radial_dist = atof ( maliste[20].valeur ) ;

  if ( maliste[21].lu == 1 ) {
    strcpy( Donnees->restart, maliste[21].valeur ) ;
    if (  strcmp( Donnees->restart, "non" ) != 0 ) {
      frestart = fopen( Donnees->restart , "r" ) ;
      if ( frestart == NULL ) {
        stop("erreur : fichier restart introuvable") ;
      } else {
        fclose ( frestart ) ;
      }
    }
  }

  if ( maliste[22].lu == 1 ) {
    sscanf ( maliste[22].valeur , "%f , %f , %f ", &tempx, &tempy, &tempz ) ;
    Donnees->vginitx = (double) tempx ;
    Donnees->vginity = (double) tempy ;
    Donnees->vginitz = (double) tempz ;
  }

  if ( maliste[23].lu == 1 ) Donnees->capacite_calo = atof ( maliste[23].valeur ) ;
  if ( maliste[24].lu == 1 ) Donnees->temp_interne0 = atof ( maliste[24].valeur ) ;
  if ( maliste[25].lu == 1 ) Donnees->freqTint      = atoi ( maliste[25].valeur ) ;
  if ( maliste[26].lu == 1 ) Donnees->sigmaDPDE     = atof ( maliste[26].valeur ) ;



  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   *                                                         *
   * avertissement concernant la valeur ou l'absence de      *
   * certains flags                                          *
   *                                                         *
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  
  if ( strcmp( Donnees->methode, "NVE" ) != 0 && strstr( Donnees->methode, "DPDE" ) == NULL ) {
     if ( maliste[3].lu == 0 ) {
       printf (                  " * ATTENTION : utilisation de la friction par defaut\n") ;
       fprintf( Donnees->cntrl , " * ATTENTION : utilisation de la friction par defaut\n") ;
     }
  }
  
  if ( strstr( Donnees->methode, "DPDE" ) != NULL ) {
       
    if ( maliste[23].lu == 0 ) {
      printf (         " * ATTENTION : utilisation du Cv par defaut pour calculer l'energie interne\n") ;
      fprintf( Donnees->cntrl , " * ATTENTION : utilisation du Cv par defaut pour calculer l'energie interne\n") ;
    }
    
    if ( maliste[24].lu == 0 ) {
      printf (                  " * ATTENTION : utilisation de la temperature interne initiale par defaut\n") ;
      fprintf( Donnees->cntrl , " * ATTENTION : utilisation de la temperature interne initiale par defaut\n") ;
    }

    if ( maliste[26].lu == 0 ) {
      printf (                  " * ATTENTION : utilisation de sigmaDPDE par defaut\n") ;
      fprintf( Donnees->cntrl , " * ATTENTION : utilisation de sigmaDPDE par defaut\n") ;
    }
    
    if ( maliste[3].lu != 0 ) {
      printf (                  " * ATTENTION : DPDE la friction est controlee par sigmaDPDE\n") ;
      fprintf( Donnees->cntrl , " * ATTENTION : DPDE la friction est controlee par sigmaDPDE\n") ;
    }
    
  }

  if ( Donnees->rcut < 2.5 * Donnees->LJsigma ) {
    printf (         " * ATTENTION : rcut < 2.5 * LJsigma \n") ;
    fprintf( Donnees->cntrl , " * ATTENTION : rcut < 2.5 * LJsigma \n") ;
  }



  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   *                                                                   *
   * Calculs et initialisation divers : volume, nat, masse volumique   *
   *                                                                   *
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

  // entete du fichier mdout 
  i = 0 ;
  fprintf ( Donnees->mdout, "# " ) ;
  for(i=0;i<70;i++) fprintf ( Donnees->mdout, "-" ) ;
  fprintf ( Donnees->mdout, "\n" ) ;
  fprintf ( Donnees->mdout, "# fichier %s \n", Donnees->amdout ) ;
  fprintf ( Donnees->mdout, "# " ) ;
  for(i=0;i<70;i++)fprintf ( Donnees->mdout, "-" ) ;
  fprintf ( Donnees->mdout, "\n" ) ;
  i = 0 ;
  fprintf ( Donnees->mdout, "# %2d : temps ecoule (fs) \n", ++i ) ;
  fprintf ( Donnees->mdout, "# %2d : energie totale par particule (J) \n", ++i ) ;
  fprintf ( Donnees->mdout, "# %2d : energie cinetique par particule (J) \n", ++i ) ;
  fprintf ( Donnees->mdout, "# %2d : energie potentielle par particule (J) \n", ++i ) ;
  fprintf ( Donnees->mdout, "# %2d : temperature (K) \n", ++i ) ;
  fprintf ( Donnees->mdout, "# %2d : pression  (Pa) \n", ++i ) ;
  fprintf ( Donnees->mdout, "# %2d : vitesse du centre de gravite sur x (m.s-1) \n", ++i ) ;
  fprintf ( Donnees->mdout, "# %2d : vitesse du centre de gravite sur y (m.s-1) \n", ++i ) ;
  fprintf ( Donnees->mdout, "# %2d : vitesse du centre de gravite sur z (m.s-1) \n", ++i ) ;
  if ( strstr( Donnees->methode, "DPDE" ) != NULL ) {
    fprintf ( Donnees->mdout, "# %2d : temperature interne (K) \n", ++i ) ;
    fprintf ( Donnees->mdout, "# %2d : energie interne par particule (J) \n", ++i ) ;
  }
  fprintf ( Donnees->mdout, "# " ) ;
  for(i=0;i<70;i++)fprintf ( Donnees->mdout, "-" ) ;
  fprintf ( Donnees->mdout, "\n" ) ;

  // calcul du nombre d'atome total
  if ( strcmp( Donnees->restart, "non" ) != 0 ) {
    // nat lu sur le fichier d'entree
    frestart = fopen ( Donnees->restart, "r") ;
    erreur += fscanf( frestart,"%d \n", &(EtatThermo->nat) ) ;
    // dimension lue sur le fichier d'entree
    erreur += fscanf( frestart,"# dimension : %e, %e, %e \n", &tempx, &tempy, &tempz) ;
    fclose( frestart );

    Donnees->boxx = (double) tempx ; 
    Donnees->boxy = (double) tempy ;
    Donnees->boxz = (double) tempz ;

  } else if ( strcmp( Donnees->cristal, "simple" ) == 0 ) {
    EtatThermo->nat = 1 * Donnees->Nx * Donnees->Ny * Donnees->Nz ;

  } else if  ( strcmp( Donnees->cristal, "centre" ) == 0 ) {
    EtatThermo->nat = 2 * Donnees->Nx * Donnees->Ny * Donnees->Nz ;

  } else if  ( strcmp( Donnees->cristal, "CFC" ) == 0 ) {
    EtatThermo->nat = 4 * Donnees->Nx * Donnees->Ny * Donnees->Nz ;
  }

  // masse
  Donnees->masse = Donnees->masse / Donnees->avogadro ;

  // dimension de la boite 
  if ( strcmp( Donnees->restart, "non" ) == 0 ) {
    Donnees->boxx = (double) Donnees->Nx * Donnees->a0 ;
    Donnees->boxy = (double) Donnees->Ny * Donnees->a0 ;
    Donnees->boxz = (double) Donnees->Nz * Donnees->a0 ;
  }
  if ( Donnees->rcut > Donnees->boxx / 2. || 
       Donnees->rcut > Donnees->boxy / 2. || 
       Donnees->rcut > Donnees->boxz / 2. ) {
    stop("erreur : rcut > dimension de la boite / 2.") ;
  }
	
  // calcul du volume
  EtatThermo->volume = Donnees->boxx * Donnees->boxy * Donnees->boxz ;

  // masse volumique
  EtatThermo->mvol = (double) EtatThermo->nat * Donnees->masse / EtatThermo->volume ;

  // parametre LJ
  Donnees->LJ4eps   = 4. * Donnees->LJeps ;
  Donnees->LJsigma6 = pow( Donnees->LJsigma , 6. ) ;
  r6       = pow( Donnees->rcut , 6. ) ;
  Donnees->LJshift  = Donnees->LJ4eps * ( Donnees->LJsigma6 / r6 )  * ( Donnees->LJsigma6 / r6 - 1. ) ;

  // sauvegarde de rcut2 et rverlet2
  Donnees->rcut2    = Donnees->rcut * Donnees->rcut ;
  Donnees->rverlet2 = Donnees->rverlet * Donnees->rverlet ;

  // distribution radiale
  Donnees->npas_radial_dist = (int) ( Donnees->rcut / Donnees->pas_radial_dist ) + 1 ;

  // deplacement max pour la liste des voisins
  Donnees->dmax2 = 0.25 * (Donnees->rcut - Donnees->rverlet) * (Donnees->rcut - Donnees->rverlet) ;

  // graine pour le random_number
  srand( Donnees->graine ) ;
  Donnees->idum = (long) Donnees->graine ;

  // coefficient Cv pour DPDE cv = 0.5 * ndbl * kB
  Donnees->capacite_calo = 0.5 * Donnees->capacite_calo * Donnees->boltzman ;

  // valeurs moyennes
  EtatThermo->ave_temp = 0. ;
  EtatThermo->sig_temp = 0. ;
  EtatThermo->ave_pres = 0. ;
  EtatThermo->sig_pres = 0. ;
  EtatThermo->ave_etot = 0. ;
  EtatThermo->sig_etot = 0. ;
  EtatThermo->ave_ektot = 0. ;
  EtatThermo->sig_ektot = 0. ;
  EtatThermo->ave_eptot = 0. ;
  EtatThermo->sig_eptot = 0. ;
  EtatThermo->ave_eitot = 0. ;
  EtatThermo->sig_eitot = 0. ;
  EtatThermo->ave_temp_int = 0.;
  EtatThermo->sig_temp_int = 0.;

  return erreur ;
}

