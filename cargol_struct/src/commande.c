
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * gestion de la ligne de commande et arret                                  * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"

extern int message_help( void ) ;

int commande( int narg, char **arg, Data *d ) {

  FILE *finput = NULL ;
  
  int erreur = EXIT_SUCCESS ;
  int i, n, id ;
  int lu[8] = {0} ;

  if ( narg == 1 || narg > 17 ) {
    printf("\nSyntaxe : %s fichier_input\n", arg[0] ) ;
    printf("%d arguments\n", narg-1 ) ;
    stop( "erreur de syntaxe dans la ligne de commande plus d'info avec -help" ) ;
  } 

/* nom des fichiers par defaut */
  strcpy( d->amdout      , "mdout.dat" ) ;
  strcpy( d->amdcrd      , "mdcrd" ) ;
  strcpy( d->amdvel      , "mdvel" ) ;
  strcpy( d->acntrl      , "md_control" ) ;
  strcpy( d->aradial_dist, "radial_dist.dat" ) ;
  strcpy( d->arestart    , "restart" ) ;
  strcpy( d->atemp_int   , "temp_interne.dat" ) ; 

  if( narg == 2 ) {
    /* 1 seul argument => input ou -help */
    
    if( strcmp( arg[1], "-help")  == 0 ||
        strcmp( arg[1], "-h")     == 0 ||
        strcmp( arg[1], "--h")    == 0 ||    
        strcmp( arg[1], "--help") == 0 ) {
      message_help() ;
    
    } else {
      /* le fichier input est le deuxieme argument : */
      strcpy( d->input, arg[1] ) ;
    }      

  } else {

    for( i = 1 ; i < narg ; i=i+2 ) {
      n  = 0 ;
      id = 0 ;

      // -i nom du fichier input : input (pas de defaut)
      if( strcmp( arg[i] , "-i" ) == 0 ) {
        if( lu[n] == 1 ) {
          stop("deux arguments -i !") ;
        } else {
          strcpy( d->input, arg[i+1] ) ;
          lu[n] = 1 ;
          id++ ;
        }
      }
      n++ ;

      // -d nom du fichier de sortie : amdout (defaut mdout.dat)
      if( strcmp( arg[i] , "-d" ) == 0 ) {
        if( lu[n] == 1 ) {
          stop("deux arguments -d !") ;
        } else {
          strcpy( d->amdout, arg[i+1] ) ;
          lu[n] = 1 ;
          id++ ;
        }
      }
      n++ ;

      // -o nom du fichier de controle des donnees : acntrl (defaut md_control)
      if( strcmp( arg[i] , "-o" ) == 0 ) {
        if( lu[n] == 1 ) {
          stop("deux arguments -o !") ;
        } else {
          strcpy( d->acntrl, arg[i+1] ) ;
          lu[n] = 1 ;
          id++ ;
        }
      }
      n++ ;

      // -x nom du fichier contenant les coordonnees : amdcrd (defaut mdcrd_NNN.xyz)
      if( strcmp( arg[i] , "-x" ) == 0 ) {
        if( lu[n] == 1 ) {
          stop("deux arguments -x !") ;
        } else {
          strcpy( d->amdcrd, arg[i+1] ) ;
          lu[n] = 1 ;
          id++ ;
        }
      }
      n++ ;
      
      // -v nom du fichier contenant les vitesses : amdvel (defaut mdvel_NNN.xyz)
      if( strcmp( arg[i] , "-v" ) == 0 ) {
        if( lu[n] == 1 ) {
          stop("deux arguments -v !") ;
        } else {
          strcpy( d->amdvel, arg[i+1] ) ;
          lu[n] = 1 ;
          id++ ;
        }
      }
      n++ ;

      // -g nom du fichier de la distribution radiale : aradial (defaut radial_dist.dat)
      if( strcmp( arg[i] , "-g" ) == 0 ) {
        if( lu[n] == 1 ) {
          stop("deux arguments -g !") ;
        } else {
          strcpy( d->aradial_dist, arg[i+1] ) ;
          lu[n] = 1 ;
          id++ ;
        }
      }
      n++ ;

      // -r nom du fichier restart (configuration finale) : arestart (defaut restart)
      if( strcmp( arg[i] , "-r" ) == 0 ) {
        if( lu[n] == 1 ) {
          stop("deux arguments -r !") ;
        } else {
          strcpy( d->arestart, arg[i+1] ) ;
          lu[n] = 1 ;
          id++ ;
        }
      }
      n++ ;

      // -t nom du fichier des temperatures internes : atemp_int (defaut temp_interne.dat)
      if( strcmp( arg[i] , "-t" ) == 0 ) {
        if( lu[n] == 1 ) {
          stop("deux arguments -t !") ;
        } else {
          strcpy( d->atemp_int, arg[i+1] ) ;
          lu[n] = 1 ;
          id++ ;
        }
      }
      n++ ;
      
      if( id == 0 ) {
        printf("Lecture de l'option %s %s\n", arg[i], arg[i+1] ) ;
        stop("Option inconnue");
      }
    }

    if( lu[0] != 1 ) stop("Option -i introuvable") ;
  }
  
  /* teste l'existance du fichier input */
  finput = fopen ( d->input , "r" ) ;
  if ( finput == NULL ) stop( "erreur : LE FICHIER D'INPUT N'EXISTE PAS ! " ) ;
  fclose( finput ) ;

  /* ouverture des fichiers de sorite recurent */
  d->mdout = fopen( d->amdout, "w" ) ;
  if ( d->mdout == NULL ) erreur++ ;

  d->cntrl = fopen( d->acntrl, "w" ) ;
  if ( d->cntrl == NULL ) erreur++ ;
  
  return erreur ;
}

