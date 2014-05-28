/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * valeur par defaut de tous les parametres                                  * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"
#include "DPDE.h"

int defaut( void ) {

  int erreur = EXIT_SUCCESS ;

// constantes
  boltzman      = 1.380662e-23 ;  	// J.K-1
  avogadro      = 6.0228e23 ;		// mol-1
  qelectron     = 1.6021892e-19 ;	// C
  aBohr         = 0.5291e-10 ;		// m
  pi            = 4. * atan(1.) ;	

// parametre du potentiel et des atomes par defaut = LJ, argon
  potentiel	= LenardJones ;
  strcpy ( aname , "Ar" ) ;
  masse	        = 39.95e-3 ;		// kg.mol-1
  LJsigma       = 3.405e-10 ;		// m
  LJeps	        = 1.6567944e-21 ;	// J

// potentiel exponnentielle 6
  Exp6A		= 1.22773e-15 ;		// J
  Exp6B		= 3.58978e10 ;		// m^-1
  Exp6C		= 9.4065e-78 ;		// J.m^6

// potentiel de morse
  Dmorse = LJeps ;			// J
  R_eq   = 3.822e-10 ;			// m
  alpha  = 1.678e10 ;			// m^-1

// controle du calcul par defaut = NVE
  strcpy ( methode ,"NVE" ) ;
  temp0	        = 300.e0 ;		// K (vitesses initiales)
  nstep         = 1000 ;
  dt            = 0.001e-12 ;		// s
  xsi           = 1e-12 ;		// kg.s-1
  // vitesse globale initiale
  vginitx = 0. ;
  vginity = 0. ;
  vginitz = 0. ;
  // nombres aleatoires
  graine		= 15041983 ;

// controle des sorties
  nave          = 5 ;
  ncrd          = 0 ;
  nout          = 10 ;
  nvel          = 0 ;
  freqTint	= 0 ;
  // distribution radiale
  pas_radial_dist = 0.1e-10 ;		// m

// cut off et voisins
  rcut          = 2.5 * LJsigma ;	// m
  rverlet       = 3.0 * LJsigma ;	// m
  nbrevoisinmax = 200 ;

// geometry initiale 
  strcpy ( cristal, "simple" ) ;
  a0            = 5.0e-10 ;		// m
  Nx            = 10 ;
  Ny            = 10 ;
  Nz            = 10 ;
  strcpy( restart, "non" ) ;		// par defaut pas de restart

// parametre specifique DPDE
  capacite_calo = 50. ;			// nombre de degres internes
  temp_interne0	= temp0 ;
  sigmaDPDE     = 1.e-17 ;		// gere la friction dans DPDE

  return erreur ;
}

