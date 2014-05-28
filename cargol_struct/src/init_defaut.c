/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * valeur par defaut de tous les parametres                                  * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"

int defaut( Data *Donnees ) {

  int   erreur = EXIT_SUCCESS ;

// constantes
  Donnees->boltzman      = 1.380662e-23 ;  	// J.K-1
  Donnees->avogadro      = 6.0228e23 ;		// mol-1
  Donnees->qelectron     = 1.6021892e-19 ;	// C
  Donnees->aBohr         = 0.5291e-10 ;		// m
  Donnees->Ang           = 1.e10 ;		// Angstrom / m
  Donnees->pi            = 4. * atan(1.) ;	

// parametre du potentiel et des atomes par defaut = argon
  strcpy ( Donnees->aname , "Ar" ) ;
  Donnees->masse	        = 39.95e-3 ;		// kg.mol-1
  Donnees->LJsigma       = 3.405e-10 ;		// m
  Donnees->LJeps	        = 1.6567944e-21 ;	// J

// controle du calcul par defaut = NVE
  strcpy ( Donnees->methode ,"NVE" ) ;
  Donnees->temp0		= 300.e0 ;		// K (vitesses initiales)
  Donnees->nstep 		= 1000 ;
  Donnees->dt 			= 0.001e-12 ;		// s
  Donnees->xsi			= 1e-12 ;		// kg.s-1
  // vitesse globale initiale
  Donnees->vginitx = 0. ;
  Donnees->vginity = 0. ;
  Donnees->vginitz = 0. ;
  // nombres aleatoires
  Donnees->graine = 15041983 ;

// controle des sorties
  Donnees->nave = 5 ;
  Donnees->ncrd = 0 ;
  Donnees->nout = 10 ;
  Donnees->nvel = 0 ;
  // distribution radiale
  Donnees->pas_radial_dist = 0.1e-10 ;		// m

// cut off et voisins
  Donnees->rcut	   = 2.5 * Donnees->LJsigma ;	// m
  Donnees->rverlet = 3.0 * Donnees->LJsigma ;	// m
  Donnees->nbrevoisinmax = 100 ;

// geometry initiale 
  strcpy ( Donnees->cristal, "simple" ) ;
  Donnees->a0 = 5.0e-10 ;		// m
  Donnees->Nx = 10 ;
  Donnees->Ny = 10 ;
  Donnees->Nz = 10 ;
  strcpy( Donnees->restart, "non" ) ;	// par defaut pas de restart

// parametre specifique DPDE
  Donnees->temp_interne0 = Donnees->temp0 ;
  Donnees->capacite_calo = 50. ;	// degres internes
  Donnees->sigmaDPDE     = 1.e-17 ;
  Donnees->freqTint      = 0 ;

  return erreur ;
}

