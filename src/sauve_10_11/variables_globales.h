/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* fichier variables_globales.h                                              */
/*                                                                           */
/* contient toutes les variables globales                                    */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* 
	on declare ici toutes les variables globales. C'est a ce niveau que
	la place en memoire leur est reservees. Dans toutes les autres fonctions
	on inclu le fichier main.h où elles sont declaree avec un extern pour
	y faire reference.
*/

/* fichiers */
char	amdout[LEN] = "mdout.dat" ;
char	amdcrd[LEN] = "mdcrd" ;
char	amdvel[LEN] = "mdvel" ;
char	acntrl[LEN] = "md_control" ;
char	aradial_dist[LEN] = "radial_dist.dat" ;
char	arestart[LEN] = "restart" ;

/* thermo */
int	nat ;
double	volume ;
double	temperature ;	// calculee pendant le run
double	temp0 ;		// donnee d'entree
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

/* potentiel Lennard Jones */
double	LJsigma ;
double	LJeps ;
double	LJsigma6 ;
double	LJ4eps ;
double	LJshift ;

/* cut off */
double	rcut, rcut2 ;
double	rverlet, rverlet2 ;
double	dmax2 ;
int	nbrevoisinmax ;

/* voisin */
int	**listevoisin = NULL ;
int	*nbrevoisin = NULL ;
int	majvoisin ;

/* donnees calcul */
char	methode[ LEN ] ;
int	nstep ;
double	dt ;
double 	xsi ;
int	nave, ncrd, nout, nvel ;

/* nombre aleatoire */
int	graine ;
/*
double	**wxrand = NULL ;
double	**wyrand = NULL ;
double	**wzrand = NULL ;
*/

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

/* DPD E */
double	*e_interne = NULL ;
double	capacite_calo ;
double	*temp_interne = NULL ;

/* constantes */
double	boltzman ;
double	avogadro ;
double	qelectron ;
double	aBohr ;
double	pi ;

