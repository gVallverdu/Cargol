
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * fichier globales.h                                                        * 
 *                                                                           * 
 * contient toutes les variables globales                                    * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* enregistrement du temps de calcul */
clock_t	start, end, t_init ;

/* thermo */
int	nat ;
double	volume ;
double	temperature ;	// calculee pendant le run
double	temp0 ;		// donnee d'entree
double  temp_interne0 ;	// temperature interne cible initiale
double	pression ;
double	massetot ;
double	mvol ;
double	virielx, viriely, virielz ;

/* moyennes */
double	ave_temp, sig_temp ;
double	ave_pres, sig_pres ;
double	ave_etot, sig_etot ;
double	ave_ektot, sig_ektot ;
double	ave_eptot, sig_eptot ;

/* atome */
double	masse ;
char 	aname[ LEN ] ;

/* potentiel */
int	potentiel ;
double	LJsigma ;
double	LJeps ;
double	LJsigma6 ;
double	LJ4eps ;
double	Exp6A, Exp6B, Exp6C ;
double	Dmorse, R_eq, alpha ;
double	Ep_shift ;

/* cut off */
double	rcut, rcut2 ;
double	rverlet, rverlet2 ;
double	dmax2 ;
int	nbrevoisinmax ;

/* voisin */
int	**listevoisin = NULL ;
int	*nbrevoisin = NULL ;
int	majvoisin ;
int	nbremajvoisin ;

/* donnees calcul */
char	methode[ LEN ] ;
int	nstep ;
double	dt ;
double 	xsi ;
int	nave, ncrd, nout, nvel ;

/* nombre aleatoire */
int	graine ;
long	idum ;
double  **wxrand = NULL , **wyrand = NULL , **wzrand = NULL ;

/* geometry initiale */
char	cristal[ LEN ] ;
double	a0 ;
int	Nx, Ny, Nz ;
double	boxx, boxy, boxz ;
char	restart[LEN] ;

/* vitesse globale */
double	vginitx, vginity, vginitz ;
double	vgx, vgy, vgz ;

/* distribution radiale */
int	npas_radial_dist ;
double	pas_radial_dist ;
double	*radial_dist = NULL ;

/* dynamique */
int	numerostep ;

double	*frcx = NULL ;
double	*frcy = NULL ;
double	*frcz = NULL ;

double	*x = NULL ;
double	*y = NULL ;
double	*z = NULL ;

double 	*vx = NULL ;
double 	*vy = NULL ;
double 	*vz = NULL ;

double 	*dx = NULL ;
double 	*dy = NULL ;
double 	*dz = NULL ;

double	etot, ektot, eptot ;

/* DPDE => DPDE.h */
int	freqTint ;
double	sigmaDPDE ;
double	capacite_calo ;
double	eitot  ;
double	ave_eitot, sig_eitot ;
double	ave_temp_int, sig_temp_int ;
double	*e_interne = NULL ;
double	*temp_interne = NULL ;

/* constantes */
double	boltzman ;
double	avogadro ;
double	qelectron ;
double	aBohr ;
double	pi ;

/* fichiers => fichiers.h */
char 	amdout[LEN] ;
char 	amdcrd[LEN] ;
char 	amdvel[LEN] ;
char 	acntrl[LEN] ;
char 	aradial_dist[LEN] ;
char 	arestart[LEN] ;
char 	atemp_int[LEN] ;

char 	input[LEN] ;

FILE	*mdout = NULL , *cntrl = NULL , *ftemp_int = NULL ;

/* end */
