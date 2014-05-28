/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* DPD energie constante :                                                   */
/* force dissipative, force aleatoire, energie et temperature interne        */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

extern	double	gaussienne( void ) ;

int force_DPDE( void ) {

  int  erreur = 0 ;

  int iat, jat, ivois, nvois ;
  double xij, yij, zij ;
  double dist, dist2 ;

  double sigma, racdt, amplitude ;
  double chi, chi2 ;
  double dvx, dvy, dvz ;
  double fdpdx, fdpdy, fdpdz ;
  double wx, wy, wz ;

  double masse_inv ;

  double Eci_old, Ecj_old, Eci, Ecj, dEci, dEcj ;
  double dEeps, var_temp ;
  double gamma_ij ;
  double vxt, vyt, vzt, ektot1, ektot2, dEeps_tot2 ;
  static double dEeps_tot = 0 ;

  sigma = sqrt( 2. * xsi * boltzman * temp0 ) ; 	// sqrt( M^2 L^2 T^-3 )
  racdt = sqrt( dt ) ;
  amplitude = sigma / racdt ;				// M L T^-2 = force

  masse_inv = dt / masse ;				// 0.5 car force dissipative et
							// aleatoire sur le 2eme 1/2 pas

  /* 
  ektot1 = 0. ;
  for ( iat = 0 ; iat < nat ; iat++ ) {
    vxt = vx[iat] - vginitx ;
    vyt = vy[iat] - vginity ;
    vzt = vz[iat] - vginitz ;

    ektot1 += 0.5 * masse * ( vxt * vxt + vyt * vyt + vzt * vzt ) ;
  }

  printf( "\n# - - - - - - - - - - - - - - - - - - - - - - \n") ;
  printf( "etot        : %e\n", ektot1 + eptot + dEeps_tot) ; */

  dEeps_tot2 = 0. ;
  for ( iat = 0 ; iat < nat-1 ; iat++ ) {

    nvois = nbrevoisin[iat] ;
    for ( ivois = 0 ; ivois < nvois ; ivois++ ) {

      jat = listevoisin[iat][ivois] ;

      // vecteur iat -> jat
      xij = x[jat] - x[iat] ;
      yij = y[jat] - y[iat] ;
      zij = z[jat] - z[iat] ;

      // image de jat la plus proche de iat
      if ( xij >  boxx / 2. ) xij = xij - boxx ;
      if ( xij < -boxx / 2. ) xij = xij + boxx ;
      if ( yij >  boxy / 2. ) yij = yij - boxy ;
      if ( yij < -boxy / 2. ) yij = yij + boxy ;
      if ( zij >  boxz / 2. ) zij = zij - boxz ;
      if ( zij < -boxz / 2. ) zij = zij + boxz ;

      // distance iat - jat au carre
      dist2 = xij * xij + yij * yij + zij * zij ;

      if ( dist2 < rcut2 ) {

        // friction pour la paire ij
        gamma_ij = 0.5 * xsi * temp0 * ( 1. / temp_interne[iat] + 1. / temp_interne[jat] ) ;

        // sauvegarde energie cinetique de i et j avant fluctuation/dissipation
        Eci_old = 0.5 * masse * ( vx[iat]*vx[iat] + vy[iat]*vy[iat] + vz[iat]*vz[iat] ) ;
        Ecj_old = 0.5 * masse * ( vx[jat]*vx[jat] + vy[jat]*vy[jat] + vz[jat]*vz[jat] ) ;

/* * * * * * * *  modification des forces pour la DPD  * * * * * * * * */

        // chi (sans unite) : fonction de cutt off simple = lineaire
        dist = sqrt(dist2) ;
        chi = 1. - dist / rcut ;
        chi2 = chi * chi ;

        // vitesse relative
        dvx = ( vx[iat] - vx[jat] ) ;
        dvy = ( vy[iat] - vy[jat] ) ;
        dvz = ( vz[iat] - vz[jat] ) ;

        // tirage
        wx = gaussienne() ;
        wy = gaussienne() ;
        wz = gaussienne() ;

        // ajout de la force de friction et de la force aleatoire
        fdpdx = - gamma_ij * chi2 * dvx + amplitude * chi * wx ;
        fdpdy = - gamma_ij * chi2 * dvy + amplitude * chi * wy ;
        fdpdz = - gamma_ij * chi2 * dvz + amplitude * chi * wz ;

        // pas de maj des forces, dissipations et fluctuations n'interviennent plus

        // maj des vitesses dues au forces dissipatives et aleatoires
        fdpdx = fdpdx * masse_inv ;
        fdpdy = fdpdy * masse_inv ;
        fdpdz = fdpdz * masse_inv ;

        vx[iat] += fdpdx ;
        vy[iat] += fdpdy ;
        vz[iat] += fdpdz ;

        vx[jat] -= fdpdx ;
        vy[jat] -= fdpdy ;
        vz[jat] -= fdpdz ;

/* * * * * * * *  energie et temperature interne : DPDE  * * * * * * * */

        // variation d'energie cinetique "a la main"
        Eci = 0.5 * masse * ( vx[iat]*vx[iat] + vy[iat]*vy[iat] + vz[iat]*vz[iat] ) ;
        Ecj = 0.5 * masse * ( vx[jat]*vx[jat] + vy[jat]*vy[jat] + vz[jat]*vz[jat] ) ;

        dEci = Eci - Eci_old ;
        dEcj = Ecj - Ecj_old ;
                  
        dEeps = - 0.5 * ( dEci + dEcj ) ;
        // dEeps_tot2 += 2. * dEeps ;

        // maj de l'energie interne
        e_interne[iat] += dEeps ;
        e_interne[jat] += dEeps ;

        /* if ( iat == 0 ) printf("%4d %4d %14.5e %14.5e %14.5e %14.5e %14.5e\n", 
                  iat, jat, dEci, dEcj, dEeps, e_interne[iat], e_interne[jat] ) ; */

        // maj de la temperature interne
        var_temp = dEeps / capacite_calo ;
        temp_interne[iat] += var_temp ;
        temp_interne[jat] += var_temp ;
      }

    } // voisin

  } // iat

  // printf(" e_interne 0 : %14.5e      ", e_interne[0] ) ;

  /*
  double einttot = 0. ;
  for ( iat = 1 ; iat < nat ; iat++ ) {
    einttot += e_interne[iat] ;
    e_interne[iat] = 0. ;
  }

  printf("einterne : %14.5e %14.5e %14.5e \n",dEeps_tot, einttot, einttot - dEeps_tot ) ;
  */


  /* // verif variation Ec = variation einterne 
  ektot2 = 0. ;
  for ( iat = 0 ; iat < nat ; iat++ ) {
    vxt = vx[iat] - vginitx ;
    vyt = vy[iat] - vginity ;
    vzt = vz[iat] - vginitz ;

    ektot2 += 0.5 * masse * ( vxt * vxt + vyt * vyt + vzt * vzt ) ;
  }

  printf( "\n APRES DPD \n") ;
  printf( "etot        : %e\n", ektot2 + eptot + dEeps_tot ) ;
  printf( "diff ektot  : %e\n", ektot2 - ektot1 ) ;
  printf( "var eint    : %e\n", dEeps_tot2 ) ;
  printf( "nesw eint   : %e\n", dEeps_tot += dEeps_tot2 ) ;
  printf( "etot + eint : %e\n", ektot2 + eptot + dEeps_tot ) ;
  */ 

  return erreur ;

}

/* calcul exact avec la formule d'Ito :

  double scal_w_dv, scal_dv_dv, dEeps ;

       // dEeps est la variation d'énergie interne pour les particules iat et jat
        scal_dv_dv = dvx * dvx + dvy * dvy + dvz * dvz ;
        scal_w_dv  = dvx * wx + dvy * wy + dvz * wz ;

        dEeps = - 0.5 * ( - gamma_ij * chi2 * scal_dv_dv * dt +
		    sigma * chi * racdt * scal_w_dv   +
		    sigma * sigma * chi2 * dt / masse ) ;
        // *1/2 car on met 1/2 sur iat et 1/2 sur jat
        // un - car ce qui est entre () est la variation d'Ec des particules

        // dans le cas ou m(iat) /= m(jat)
        denergie = - gamma_ij * chi2 * scal_dv_dv * dt +
                     sigma * chi * racdt * scal_w_dv   +
                     0.5 * ( 1. / m[iat] + 1. / m[jat] ) * sigma * sigma * chi2 * dt  ;




        // tire du code de gabriel stamp2 : computeForceDPD.c

        // DPD avec un pas complet sur les forces : on a fait le *dt
        fdpdx = - gamma_ij * chi2 * dvx * dt + sigma * racdt * chi * wx ;
        fdpdy = - gamma_ij * chi2 * dvy * dt + sigma * racdt * chi * wy ;
        fdpdz = - gamma_ij * chi2 * dvz * dt + sigma * racdt * chi * wz ;

        denergie  = fdpdx * dvx + fdpdy * dvy + fdpdz * dvz ;
        // sur ces termes faire *3 car le terme est pour x y et z ?
        denergie += sigma * sigma * chi2 * dt / masse ;
        // si iat et jat n'ont pas la même masse :
        denergie += 0.5 * ( 1./masse[iat] + 1./masse[jat] ) * sigma * sigma * chi2 * dt  ;

        dEeps = - 0.5 * denergie ;
        // *1/2 car on met 1/2 sur iat et 1/2 sur jat
        // un - car ce qui c'est la variation d'Ec des 2 particules

*/

