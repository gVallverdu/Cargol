
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * generateurs de nombre aleatoires                                          * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>
#include <math.h>

 /*
  * rand() est le generateur de nombres aleatoires uniformes entre 
  * 0 et 1 de la librairie C math.h. La graine est initialisée 
  * avec la fonction srand ( unsigned int ) de math.h
  *
  * NumRec suggere de ne pas utiliser cette fonction. Le generateur
  * ran1( long *) de NumRec est le plus robuste des versions simples
  * et efficace de generateur de nombres aleatoires de NumRec.
  *
  * Pour une distribution gaussienne de nombres aleatoires, 
  * gaussienne et gasdev sont de qualite similaire. gasdev
  * est deux fois plus rapide que gaussienne.
  *
  */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * rand() : librairie C                                                      * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

double random_number(void) {
  return (double) rand() / RAND_MAX ;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           * 
 * ran0 : Numerical Recipes p 279                                            * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define MASK 123459876

double ran0 ( long *idum ) {
  long   k;
  double ans;
  *idum ^= MASK ;				/* XORing with MASK allows use of zero and
                                                   other simple bit patterns for idum */
  k = *idum / IQ;				
  *idum = IA * ( *idum - k * IQ ) - IR * k ;	/* compute idum without overflows
                                                   schrage's method idum=(IA*idum)%IM */
  if( *idum < 0 ) *idum += IM;
  ans = AM * ( *idum ) ;
  *idum ^= MASK;				/* Unmask before return */
  return ans ;
}

#undef IA
#undef IM
#undef AM
#undef IQ
#undef IR
#undef MASK

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * ran1 : Numerical Recipes p 281                                            * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS (1.2E-07)
#define RNMX (1.0-EPS) 

double ran1 ( long *idum ) {
  /*
   * idum must be set as a negative integer and must never be 
   * modified elsewhere. User have to set its value before the 
   * first call to this function.
   *
   */
  int    j;
  long   k;
  double temp;
  static long iy=0;
  static long iv[NTAB];

  if ( (*idum <= 0) || (iy == 0) ) {		// initialize
    if (-(*idum) < 1 ) {			// be sure to prevent idum = 0
      *idum = 1 ;
    } else {
      *idum = -(*idum);
    }
    for(j=NTAB+7;j>=0;j--) {			// load the shuffle table (after 8 warm ups)
      k = *idum/IQ;
      *idum = IA*(*idum-k*IQ)-IR*k;
      if(*idum < 0) *idum += IM;
      if(j < NTAB) iv[j] = *idum;
    }
    iy = iv[0];
  }
  k = *idum / IQ;				// start here when not initializing
  *idum = IA * ( *idum - k * IQ ) - IR * k ;	// compute idum without overflows
  if( *idum < 0 ) *idum += IM ;			// 	schrage's method idum=(IA*idum)%IM
  j = iy / NDIV ;				// will be in the range 0 ... NTAB-1
  iy = iv[j] ;					// Output previously stored value and 
  iv[j] = *idum;				//	refill the shuffle table
  if ( (temp = AM * iy ) > RNMX ) {		// because users don't expect endpoint val
    return RNMX ;
  } else {
    return temp ;
  }
}
#undef IA 
#undef IM 
#undef AM 
#undef IQ 
#undef IR 
#undef NTAB 
#undef NDIV 
#undef EPS 
#undef RNMX

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           * 
 * ran2 : Numerical Recipes p 282                                            * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

#define IM1 2147483563
#define IM2 2147483399
#define AM (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NTAB 32
#define NDIV (1+IMM1/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

double ran2(long *idum) {
	int j;
	long k;
	static long idum2=123456789;
	static long iy=0;
	static long iv[NTAB];
	double temp;

	if (*idum <= 0) {
		if (-(*idum) < 1) *idum=1;
		else *idum = -(*idum);
		idum2=(*idum);
		for (j=NTAB+7;j>=0;j--) {
			k=(*idum)/IQ1;
			*idum=IA1*(*idum-k*IQ1)-k*IR1;
			if (*idum < 0) *idum += IM1;
			if (j < NTAB) iv[j] = *idum;
		}
		iy=iv[0];
	}
	k=(*idum)/IQ1;
	*idum=IA1*(*idum-k*IQ1)-k*IR1;
	if (*idum < 0) *idum += IM1;
	k=idum2/IQ2;
	idum2=IA2*(idum2-k*IQ2)-k*IR2;
	if (idum2 < 0) idum2 += IM2;
	j=iy/NDIV;
	iy=iv[j]-idum2;
	iv[j] = *idum;
	if (iy < 1) iy += IMM1;
	if ((temp=AM*iy) > RNMX) return RNMX;
	else return temp;
}
#undef IM1
#undef IM2
#undef AM
#undef IMM1
#undef IA1
#undef IA2
#undef IQ1
#undef IQ2
#undef IR1
#undef IR2
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * distribution gaussienne : gabriel stoltz                                  * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

double gaussienne (void) {

  double t, r1, r2, res1, res2 ;

  t  = random_number();
  r1 = random_number();

  if ( r1 == 0 ) r1 = 1e-9 ;

  r2 =  random_number();

  if (t < .5){
    res1 = sqrt(-2.*log(r1))*cos(2.*M_PI*r2);
    return (res1);
  } else {
    res2 = sqrt(-2.*log(r1))*sin(2.*M_PI*r2);
    return (res2);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

double gauss( void ) {
  double r1, r2, res1 ;
  r1 = random_number() ;
  if ( r1 == 0 ) r1 = 1e-9 ;
  r2 =  random_number() ;
  res1 = sqrt(-2.*log(r1))*cos(2.*M_PI*r2);
  return (res1);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * gasdev: Numerical recipes p 289                                           * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

double gasdev ( long *idum ) {
  static int    iset = 0 ;
  static double gset ;
  double        fac, rsq, v1, v2 ;
  if (iset == 0) {
    do {
      v1 = 2.0 * ran1(idum) - 1.0 ;
      v2 = 2.0 * ran1(idum) - 1.0 ;
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 


