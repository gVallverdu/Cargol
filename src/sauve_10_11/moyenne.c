/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* calcul de grandeurs moyennes                                              */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

int moyenne ( int step ) {

	int 	erreur = 0 ;
	int	iat ;
	double	ecinet ;
	double	vxt, vyt, vzt ;
	double	viriel ;
	double	conv ;

	FILE *mdout ;

	ecinet = 0 ;
	vgx = vgy = vgz = 0. ;
	for ( iat = 0 ; iat < nat ; iat++ ) {

		vxt = vx[iat] - vginitx ;
		vyt = vy[iat] - vginity ;
		vzt = vz[iat] - vginitz ;

		// calcul de l'energie cinetique
		ecinet += 0.5 * masse * ( vxt * vxt + vyt * vyt + vzt * vzt ) ;

		// vitesse du centre de gravite
		vgx += masse * vx[iat] ;
		vgy += masse * vy[iat] ;
		vgz += masse * vz[iat] ;
	}

	// vitesse du centre de gravite
	vgx = vgx / massetot ;
	vgy = vgy / massetot ;
	vgz = vgz / massetot ;

	// temperature
	temperature = 2. * ecinet / ( 3. * (double) nat * boltzman ) ;

	// pression
	viriel = virielx + viriely + virielz ;
	pression = 2.*ecinet / ( 3.*volume ) + viriel / ( 3.*volume ) ;

	// enertie totale
	ektot 		= ecinet ; 
	etot 		= ektot + eptot ;

	// impression dans le fichier amdout
	mdout = fopen( amdout, "a" );
	fprintf ( mdout, "%10d", step ) ;
	fprintf ( mdout, "%14.6e", etot  / (double) nat ) ;
	fprintf ( mdout, "%14.6e", ektot / (double) nat ) ;
	fprintf ( mdout, "%14.6e", eptot / (double) nat ) ;
	fprintf ( mdout, "%14.6e", temperature ) ;
	fprintf ( mdout, "%14.6e", pression ) ;
	fprintf ( mdout, "%14.6e", vgx ) ;
	fprintf ( mdout, "%14.6e", vgy ) ;
	fprintf ( mdout, "%14.6e", vgz ) ;
	fprintf ( mdout, "\n" ) ;
	fclose(mdout) ;

	// affichage a l'ecran
	if ( ( nout != 0 ) && ( step % nout == 0 || step == 1 ) ) {
		fprintf ( stdout, "%8d", step );
		conv = avogadro / ( 1000. * (double) nat ) ;
		fprintf ( stdout, "   E = %11.4e" , etot  * conv ) ;
		fprintf ( stdout, "   Ec = %11.4e", ektot * conv ) ;
		fprintf ( stdout, "   Ep = %11.4e", eptot * conv ) ;
		fprintf ( stdout, "   T = %10.3f", temperature ) ;
		fprintf ( stdout, "   P = %11.4e\n", pression ) ;
	}

	// calculs des valeurs moyennes et fluctuations
	ave_temp += temperature ;
	sig_temp += temperature * temperature ;

	ave_pres += pression ;
	sig_pres += pression * pression ;

	ave_etot += etot ;
	sig_etot += etot * etot ;

	ave_ektot += ektot ;
	sig_ektot += ektot * ektot ;

	ave_eptot += eptot ;
	sig_eptot += eptot * eptot ;

	return erreur;
}

