/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* fichier extern.h                                                          */
/*                                                                           */
/* contient les variables globales pour y acceder                            */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* fichiers */
extern	char	amdout[LEN] ;
extern	char	amdcrd[LEN] ;
extern	char	amdvel[LEN] ;
extern	char	acntrl[LEN] ;
extern	char	aradial_dist[LEN] ;
extern	char	arestart[LEN] ;

/* thermo */
extern	int	nat ;
extern	double	volume ;
extern	double	temperature ;	// calculee pendant le run
extern	double	temp0 ;		// donnee d'entre
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

/* potentiel Lennard Jones */
extern	double	LJsigma ;
extern	double	LJeps ;
extern	double	LJsigma6 ;
extern	double	LJ4eps ;
extern	double	LJshift ;

/* nombre aleatoire */
extern	int	graine ;
// extern	double	**wxrand, **wyrand, **wzrand ;

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

/* dynamique : position, vitesse, deplacement, force, energie */
extern	double *x, *y, *z ;
extern	double *vx, *vy, *vz ;
extern	double *dx, *dy, *dz ;
extern	double *frcx, *frcy, *frcz ;
extern	double	etot, ektot, eptot ;

/* DPD E */
extern	double	*e_interne ;
extern	double	capacite_calo ;
extern	double	*temp_interne ;

/* constantes physiques ou mathematiques */
extern	double	boltzman ;
extern	double	avogadro ;
extern	double	qelectron ;
extern	double	aBohr ;
extern	double	pi ;

