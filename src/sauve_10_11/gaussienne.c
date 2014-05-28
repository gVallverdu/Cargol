/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* variable aleatoire gaussienne - Version G. Stoltz 07/2005                 */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>
#include <math.h>

/* generateur de variable aleatoire uniforme entre 0 et 1 */
double random_number(void) {
	double temp ;
	
	temp = (double) rand() / RAND_MAX ;

	return temp;
}

/* generateur de gaussienne (methode polaire : Gabriel) */
double gauss_G (void) {
	double	t, r1, r2, res1, res2 ;

	t  = random_number();
	r1 = random_number();

	if ( r1 == 0 ) r1 = 1e-9 ;

	r2 =  random_number();

	if (t < .5){
		res1 = sqrt(-2.*log(r1))*cos(2.*M_PI*r2);
		return (res1);
	}else{
		res2 = sqrt(-2.*log(r1))*sin(2.*M_PI*r2);
		return (res2);
	}
}

/* gasdev: NumRec page 289 */
double gasdev ( void ) {
	static	int	iset = 0 ;
	static 	double	gset ;
	double 	fac, rsq, v1, v2 ;

	if (iset == 0) {
		do {
			v1 = 2.0 * random_number() - 1.0 ;
			v2 = 2.0 * random_number() - 1.0 ;
			rsq = v1*v1 + v2*v2 ;
		} while ( rsq >= 1.0 || rsq == 0.0 ) ;
		fac = sqrt( -2.0 * log(rsq) / rsq ) ;
		gset = v1 * fac ;
		iset = 1 ;
		return (double) ( v2 * fac ) ;
	} else {
		iset = 0 ;
		return (double) gset ;
	}
}

