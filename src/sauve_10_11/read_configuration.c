/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* lecture de la configuration initiale                                      */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"
#include "extern.h"

int read_configuration( void ) {

	int	erreur = 0 ;
	int	i ;
	char	ligne[ LEN ] = "" ;
	float	tempx, tempy, tempz ;
	double	ecinet, vxt, vyt, vzt ;
	FILE *	inpcrd ;
	FILE *	frestart ;

	// ouverture du fichier restart
	frestart = fopen( restart, "r" ) ;

	fgets( ligne, LEN, frestart ) ;  // nat
	fgets( ligne, LEN, frestart ) ;  // dimension

	// lecture positions, conversion A -> m
	for ( i = 0 ; i < nat ; i++ ) {
		fscanf( frestart,"%*s %e %e %e\n", &tempx, &tempy, &tempz ) ;
		x[i] = (double) tempx * 1.e-10 ;
		y[i] = (double) tempy * 1.e-10 ;
		z[i] = (double) tempz * 1.e-10 ;
	}

	fgets( ligne, LEN, frestart ) ;  // titre vitesse

	// lecture vitesses
	vgx = vgy = vgz = 0. ;
	massetot = 0. ;
	for ( i = 0 ; i < nat ; i++ ) {
		fscanf( frestart,"%*s %e %e %e\n", &tempx, &tempy, &tempz ) ;
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

	vginitx = vgx ;
	vginity = vgy ;
	vginitz = vgz ;

	ecinet = 0. ;
	for ( i = 0 ; i < nat ; i++ ) {

		vxt = vx[i] - vginitx ;
		vyt = vy[i] - vginity ;
		vzt = vz[i] - vginitz ;

		// calcul de l'energie cinetique
		ecinet += 0.5 * masse * ( vxt * vxt + vyt * vyt + vzt * vzt ) ;
	}
	temperature = 2. * ecinet / ( 3. * (double) nat * boltzman ) ;
	ektot = ecinet ;

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

