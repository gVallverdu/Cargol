
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * fichier extern.h                                                          * 
 *                                                                           * 
 * contient les variables globales pour y acceder                            * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* enregistrement du temps de calcul */
extern	clock_t	start, end, t_init ;

/* thermo */
extern	int	nat ;
extern	double	volume ;
extern	double  temperature ;   // calculee pendant le run
extern	double	temp0 ;		// donnee d'entre
extern	double	temp_interne0 ;	// temperature interne cible initiale
extern	double	pression ;
extern	double	massetot ;
extern 	double	mvol ;
extern	double	virielx, viriely, virielz ;

/* moyennes */
extern	double	ave_temp, sig_temp ;
extern	double	ave_pres, sig_pres ;
extern	double	ave_etot, sig_etot ;
extern	double	ave_ektot, sig_ektot ;
extern	double	ave_eptot, sig_eptot ;

/* donnees calcul */
extern	char	methode[LEN] ;
extern	int	nstep ;
extern	double	dt ;
extern	double 	xsi ;
extern	int	nave, ncrd, nout, nvel ;

/* atome */
extern	double	masse ;
extern	char 	aname[LEN] ;

/* potentiel */
extern	int	potentiel ;
extern	double	LJsigma ;
extern	double	LJeps ;
extern	double	LJsigma6 ;
extern	double	LJ4eps ;
extern	double	Exp6A, Exp6B, Exp6C ;
extern	double	Dmorse, R_eq, alpha ;
extern	double	Ep_shift ;

/* nombre aleatoire */
extern	int	graine ;
extern	double	**wxrand, **wyrand, **wzrand ;
extern	long	idum ;

/* geometry initiale */
extern	char	cristal[LEN] ;
extern	double	a0 ;
extern	int	Nx, Ny, Nz ;
extern	double	boxx, boxy, boxz ;
extern	char	restart[LEN] ;

/* vitesse globale */
extern	double	vginitx, vginity, vginitz ;
extern	double	vgx, vgy, vgz ;

/* distribution radiale */
extern	int	npas_radial_dist ;
extern	double	pas_radial_dist ;
extern	double	*radial_dist ;

/* cut off */
extern	double	rcut, rcut2 ;
extern	double	rverlet, rverlet2 ;
extern	double	dmax2 ;			// (rcut - rverlet)^2
extern	int	nbrevoisinmax ;

/* voisin */
extern	int	**listevoisin ;
extern	int	*nbrevoisin ;
extern	int	majvoisin ;
extern	int	nbremajvoisin ;

/* dynamique : position, vitesse, deplacement, force, energie */
extern	int	numerostep ;
extern	double	*x, *y, *z ;
extern	double	*vx, *vy, *vz ;
extern	double	*dx, *dy, *dz ;
extern	double	*frcx, *frcy, *frcz ;
extern	double	etot, ektot, eptot ;

/* constantes physiques ou mathematiques */
extern	double	boltzman ;
extern	double	avogadro ;
extern	double	qelectron ;
extern	double	aBohr ;
extern	double	pi ;

