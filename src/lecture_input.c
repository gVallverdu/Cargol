
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * lecture du fichier input                                                  * 
 *                                                                           * 
 * test des valeurs lues                                                     * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"
#include "DPDE.h"
#include "fichier.h"

int lecture_input( void ) {

  struct namelist maliste ;

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
  strcpy( maliste.flag[ 0] , "methode" ) ;
  strcpy( maliste.flag[ 1] , "nstep" ) ;
  strcpy( maliste.flag[ 2] , "dt" ) ;
  strcpy( maliste.flag[ 3] , "xsi" ) ;
  strcpy( maliste.flag[ 4] , "temp0" ) ;
  strcpy( maliste.flag[ 5] , "rcut" ) ;
  strcpy( maliste.flag[ 6] , "rverlet" ) ; 
  // nombre aleatoires
  strcpy( maliste.flag[ 7] , "graine" ) ;
  // parametre du potentiel et des atomes
  strcpy( maliste.flag[ 8] , "aname" ) ;
  strcpy( maliste.flag[ 9] , "masse" ) ;
  strcpy( maliste.flag[10] , "LJsigma" ) ;
  strcpy( maliste.flag[11] , "LJeps" ) ;
  // parametres geometries initiales
  strcpy( maliste.flag[12] , "cristal" ) ;
  strcpy( maliste.flag[13] , "a0" ) ;
  strcpy( maliste.flag[14] , "dimension" ) ;
  // voisin
  strcpy( maliste.flag[15] , "nbrevoisinmax" ) ;
  // frequence output
  strcpy( maliste.flag[16] , "nave" ) ;
  strcpy( maliste.flag[17] , "ncrd" ) ;
  strcpy( maliste.flag[18] , "nout" ) ;
  strcpy( maliste.flag[19] , "nvel" ) ;
  // pas en R pour le calcul de g(r)
  strcpy( maliste.flag[20] , "pas_radial_dist" ) ;
  // restart
  strcpy( maliste.flag[21] , "restart" ) ;
  // vitesse globale initiale
  strcpy( maliste.flag[22] , "vg_initiale" ) ;
  // pour la DPDE
  strcpy( maliste.flag[23] , "capacite_calo" ) ;
  strcpy( maliste.flag[24] , "temp_interne0" ) ;
  strcpy( maliste.flag[25] , "freqTint" ) ;  
  strcpy( maliste.flag[26] , "sigmaDPDE" ) ;  
  // potentiel
  strcpy( maliste.flag[27] , "potentiel" ) ;
  // exponnentielle 6
  strcpy( maliste.flag[28] , "Exp6A" ) ;
  strcpy( maliste.flag[29] , "Exp6B" ) ;
  strcpy( maliste.flag[30] , "Exp6C" ) ;
  // Morse
  strcpy( maliste.flag[31] , "Dmorse" ) ;
  strcpy( maliste.flag[32] , "R_eq" ) ;
  strcpy( maliste.flag[33] , "alpha" ) ;
  
  // pour savoir si le flag a ete lu ou pas
  for ( i = 0 ; i < NFLAG ; i++ ) maliste.lu[i] = 0 ; 

  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   *                                                       *
   *             lecture du fichier d'input                *
   *                                                       *
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

  // fichier contenant la namelist
  finput = fopen ( input , "r" ) ;

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

      if ( strstr( ligne, maliste.flag[i] ) != NULL ) {

        // on a identifie un flag 
        if ( maliste.lu[i] == 1 ) {
          printf("\n\nflag %s, ", maliste.flag[i] ) ;
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
          maliste.lu[i] = 1 ;
          existe = 1 ;
          sscanf ( ligne , "%*s %s", maliste.valeur[i] ) ;
          break ;
        }
      }
    }

    if ( existe == 0 ) {
      printf("\n\nErreur lors de la lecture du fichier input : %s\n", input ) ;
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

  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   *                                                         *
   * si un flag a ete lu on attribue la valeur a la variable *
   * qui lui correspond dans variables_globales.h            *
   *                                                         *
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

  if ( maliste.lu[0] == 1 ) {
    strcpy( methode, maliste.valeur[0] ) ;
    if ( strcmp( methode, "NVE" )          != 0 &&
         strcmp( methode, "langevin" )     != 0 &&
         strcmp( methode, "LNVT" )         != 0 &&
         strcmp( methode, "DPD" )          != 0 &&
         strcmp( methode, "DPDnosplit" )   != 0 &&
         strcmp( methode, "DPDE_alamain" ) != 0 &&
         strcmp( methode, "DPDE_ito" )     != 0 &&
         strcmp( methode, "DPDE" )         != 0 ) 
    {
      stop(" erreur flag methode : type de calcul inconnu" ) ;
    }
  }

  if ( maliste.lu[ 1] == 1 ) nstep   = atoi ( maliste.valeur[ 1] ) ;
  if ( maliste.lu[ 2] == 1 ) dt      = atof ( maliste.valeur[ 2] ) ;
  if ( maliste.lu[ 3] == 1 ) xsi     = atof ( maliste.valeur[ 3] ) ;
  if ( maliste.lu[ 4] == 1 ) temp0   = atof ( maliste.valeur[ 4] ) ;
  if ( maliste.lu[ 5] == 1 ) rcut    = atof ( maliste.valeur[ 5] ) ;
  if ( maliste.lu[ 6] == 1 ) rverlet = atof ( maliste.valeur[ 6] ) ;
  if ( maliste.lu[ 7] == 1 ) graine  = atoi ( maliste.valeur[ 7] ) ;
  if ( maliste.lu[ 8] == 1 ) strcpy ( aname , maliste.valeur[ 8] ) ;
  if ( maliste.lu[ 9] == 1 ) masse   = atof ( maliste.valeur[ 9] ) ;
  if ( maliste.lu[10] == 1 ) LJsigma = atof ( maliste.valeur[10] ) ;
  if ( maliste.lu[11] == 1 ) LJeps   = atof ( maliste.valeur[11] ) ;

  if ( maliste.lu[12] == 1 ) {
    strcpy( cristal, maliste.valeur[12] ) ;
    if ( strcmp( cristal, "simple" ) != 0 && 
         strcmp( cristal, "centre" ) != 0 &&
         strcmp( cristal, "CFC" )    != 0 ) {
      stop("erreur flag cristal : type de maille elementaire inconnu") ;
    }
  }

  if ( maliste.lu[13] == 1 ) a0 = atof ( maliste.valeur[13] ) ;

  if ( maliste.lu[14] == 1 ) {
    sscanf ( maliste.valeur[14] , "%d , %d , %d", &Nx, &Ny, &Nz ) ;
    if ( Nx == 0 || Ny == 0 || Nz == 0 ) {
      stop("erreur dimension : les dimensions doivent etre > ou = a 1\n\n pas d'espace entre les dimensions : 10,10,10" ) ;
    }
  }

  if ( maliste.lu[15] == 1 ) nbrevoisinmax = atoi ( maliste.valeur[15] ) ;
  if ( maliste.lu[16] == 1 ) nave = atoi ( maliste.valeur[16] ) ;
  if ( maliste.lu[17] == 1 ) ncrd = atoi ( maliste.valeur[17] ) ;
  if ( maliste.lu[18] == 1 ) nout = atoi ( maliste.valeur[18] ) ;
  if ( maliste.lu[19] == 1 ) nvel = atoi ( maliste.valeur[19] ) ;
  if ( maliste.lu[20] == 1 ) pas_radial_dist = atof ( maliste.valeur[20] ) ;

  if ( maliste.lu[21] == 1 ) {
    strcpy( restart, maliste.valeur[21] ) ;
    if (  strcmp( restart, "non" ) != 0 ) {
      frestart = fopen( restart , "r" ) ;
      if ( frestart == NULL ) {
        stop("erreur : fichier restart introuvable") ;
      } else {
        fclose ( frestart ) ;
      }
    }
  }

  if ( maliste.lu[22] == 1 ) {
    sscanf ( maliste.valeur[22] , "%f , %f , %f ", &tempx, &tempy, &tempz ) ;
    vginitx = (double) tempx ;
    vginity = (double) tempy ;
    vginitz = (double) tempz ;
  }

  if ( maliste.lu[23] == 1 ) capacite_calo = atof ( maliste.valeur[23] ) ;
  if ( maliste.lu[24] == 1 ) temp_interne0 = atof ( maliste.valeur[24] ) ;
  if ( maliste.lu[25] == 1 ) freqTint      = atoi ( maliste.valeur[25] ) ;
  if ( maliste.lu[26] == 1 ) sigmaDPDE     = atof ( maliste.valeur[26] ) ;

  if ( maliste.lu[27] == 1 ) {
    if ( strcmp( maliste.valeur[27], "LenardJones" ) == 0 ) {
      potentiel = LenardJones ;
    } else if ( strcmp( maliste.valeur[27], "Exp6" ) == 0 ) {
      potentiel = Exp6 ;
    } else if ( strcmp( maliste.valeur[27], "Morse" ) == 0 ) {
      potentiel = Morse ;
    } else {
      stop("erreur flag potentiel : potentiel inconu" );
    }
  }

  if ( maliste.lu[28] == 1 ) Exp6A   = atof ( maliste.valeur[28] ) ;
  if ( maliste.lu[29] == 1 ) Exp6B   = atof ( maliste.valeur[29] ) ;
  if ( maliste.lu[30] == 1 ) Exp6C   = atof ( maliste.valeur[30] ) ;
  if ( maliste.lu[31] == 1 ) Dmorse  = atof ( maliste.valeur[31] ) ;
  if ( maliste.lu[32] == 1 ) R_eq    = atof ( maliste.valeur[32] ) ;
  if ( maliste.lu[33] == 1 ) alpha   = atof ( maliste.valeur[33] ) ;

  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   *                                                         *
   * avertissement concernant la valeur ou l'absence de      *
   * certains flags                                          *
   *                                                         *
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  
  if ( strcmp( methode, "NVE" ) != 0 && strstr( methode, "DPDE" ) == NULL ) {
     if ( maliste.lu[3] == 0 ) {
       printf (         " * ATTENTION : utilisation de la friction par defaut\n") ;
       fprintf( cntrl , " * ATTENTION : utilisation de la friction par defaut\n") ;
     }
  }
  
  if ( strstr( methode, "DPDE" ) != NULL ) {
       
    if ( maliste.lu[23] == 0 ) {
      printf (         " * ATTENTION : utilisation du Cv par defaut pour calculer l'energie interne\n") ;
      fprintf( cntrl , " * ATTENTION : utilisation du Cv par defaut pour calculer l'energie interne\n") ;
    }
    
    if ( maliste.lu[24] == 0 ) {
      printf (         " * ATTENTION : utilisation de la temperature interne initiale par defaut\n") ;
      fprintf( cntrl , " * ATTENTION : utilisation de la temperature interne initiale par defaut\n") ;
    }
    
    if ( maliste.lu[26] == 0 ) {
      printf (         " * ATTENTION : utilisation de sigmaDPDE par defaut\n") ;
      fprintf( cntrl , " * ATTENTION : utilisation de sigmaDPDE par defaut\n") ;
    }
    
    if ( maliste.lu[3] != 0 ) {
      printf (         " * ATTENTION : DPDE la friction est controlee par sigmaDPDE\n") ;
      fprintf( cntrl , " * ATTENTION : DPDE la friction est controlee par sigmaDPDE\n") ;
    }
    
  }

  if ( rcut < 2.5 * LJsigma ) {
    printf (         " * ATTENTION : rcut < 2.5 * LJsigma \n") ;
    fprintf( cntrl , " * ATTENTION : rcut < 2.5 * LJsigma \n") ;
  }



  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   *                                                         *
   * Calculs divers : volume, nat, masse volumique           *
   *                                                         *
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

  // entete du fichier mdout 
  i = 0 ;
  fprintf ( mdout, "# " ) ;
  for(i=0;i<70;i++) fprintf ( mdout, "-" ) ;
  fprintf ( mdout, "\n" ) ;
  fprintf ( mdout, "# fichier %s \n", amdout ) ;
  fprintf ( mdout, "# " ) ;
  for(i=0;i<70;i++)fprintf ( mdout, "-" ) ;
  fprintf ( mdout, "\n" ) ;
  i = 0 ;
  fprintf ( mdout, "# %2d : temps ecoule (fs) \n", ++i ) ;
  fprintf ( mdout, "# %2d : energie totale par particule (J) \n", ++i ) ;
  fprintf ( mdout, "# %2d : energie cinetique par particule (J) \n", ++i ) ;
  fprintf ( mdout, "# %2d : energie potentielle par particule (J) \n", ++i ) ;
  fprintf ( mdout, "# %2d : temperature (K) \n", ++i ) ;
  fprintf ( mdout, "# %2d : pression  (Pa) \n", ++i ) ;
  fprintf ( mdout, "# %2d : vitesse du centre de gravite sur x (m.s-1) \n", ++i ) ;
  fprintf ( mdout, "# %2d : vitesse du centre de gravite sur y (m.s-1) \n", ++i ) ;
  fprintf ( mdout, "# %2d : vitesse du centre de gravite sur z (m.s-1) \n", ++i ) ;
  if ( strstr( methode, "DPDE" ) != NULL ) {
    fprintf ( mdout, "# %2d : temperature interne (K) \n", ++i ) ;
    fprintf ( mdout, "# %2d : energie interne par particule (J) \n", ++i ) ;
  }
  fprintf ( mdout, "# " ) ;
  for(i=0;i<70;i++)fprintf ( mdout, "-" ) ;
  fprintf ( mdout, "\n" ) ;

  // calcul du nombre d'atome total
  if ( strcmp( restart, "non" ) != 0 ) {
    // nat lu sur le fichier d'entree
    frestart = fopen ( restart, "r") ;
    erreur += fscanf( frestart,"%d \n", &nat ) ;
    // dimension lue sur le fichier d'entree
    erreur += fscanf( frestart,"# dimension : %e, %e, %e \n", &tempx, &tempy, &tempz) ;
    fclose( frestart );

    boxx = (double) tempx ;
    boxy = (double) tempy ;
    boxz = (double) tempz ;

  } else if ( strcmp( cristal, "simple" ) == 0 ) {
    nat = 1 * Nx * Ny * Nz ;

  } else if  ( strcmp( cristal, "centre" ) == 0 ) {
    nat = 2 * Nx * Ny * Nz ;

  } else if  ( strcmp( cristal, "CFC" ) == 0 ) {
    nat = 4 * Nx * Ny * Nz ;
  }

  // masse
  masse = masse / avogadro ;

  // dimension de la boite 
  if ( strcmp( restart, "non" ) == 0 ) {
    boxx   = (double) Nx * a0 ;
    boxy   = (double) Ny * a0 ;
    boxz   = (double) Nz * a0 ;
  }
  if ( rcut > boxx / 2. || rcut > boxy / 2. || rcut > boxz / 2. ) {
    stop("erreur : rcut > dimension de la boite / 2.") ;
  }
	
  // calcul du volume
  volume = boxx * boxy * boxz ;

  // masse volumique
  mvol = (double) nat * masse / volume ;

  // parametre LJ
  LJ4eps   = 4. * LJeps ;
  LJsigma6 = pow( LJsigma , 6. ) ;
  r6       = pow( rcut , 6. ) ;

  if( potentiel == LenardJones ) {
    Ep_shift  = LJ4eps * ( LJsigma6 / r6 )  * ( LJsigma6 / r6 - 1. ) ;

  } else if( potentiel == Exp6 ) {
    Ep_shift = Exp6A * exp( - Exp6B * rcut ) - Exp6C / r6 ;

  } else if( potentiel == Morse ) {
    Ep_shift = Dmorse * pow( 1. - exp( - alpha * (rcut - R_eq)) , 2 ) ;

  } else {
    stop("erreur du potentiel") ;

  }

  // sauvegarde de rcut2 et rverlet2
  rcut2    = rcut * rcut ;
  rverlet2 = rverlet * rverlet ;

  // distribution radiale
  npas_radial_dist = (int) ( rcut / pas_radial_dist ) + 1 ;

  // deplacement max pour la liste des voisins
  dmax2 = 0.25 * (rcut - rverlet) * (rcut - rverlet) ;

  // graine pour le random_number
  srand( graine ) ;
  idum = (long) graine ;

  // coefficient Cv pour DPDE cv = 0.5 * ndbl * kB
  capacite_calo = 0.5 * capacite_calo * boltzman ;

  // valeurs moyennes
  ave_temp = 0. ;
  sig_temp = 0. ;
  ave_pres = 0. ;
  sig_pres = 0. ;
  ave_etot = 0. ;
  sig_etot = 0. ;
  ave_ektot = 0. ;
  sig_ektot = 0. ;
  ave_eptot = 0. ;
  sig_eptot = 0. ;
  ave_eitot = 0. ;
  sig_eitot = 0. ;
  ave_temp_int = 0.;
  sig_temp_int = 0.;

  return erreur ;
}

