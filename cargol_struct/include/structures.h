/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * structures                                                                * 
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 

/* * * * * structure namelist */
struct namelist {
  char  flag[LEN] ;
  char  valeur[LEN] ;
  int   lu ;
} ;
typedef struct namelist Namelist ;


/* * * * * structure particule */
struct part {
  double  x, y, z ;
  double  vx, vy, vz ;
  double  frcx, frcy, frcz ;
  double  dx, dy, dz ;
  int	  nbrevoisin ;
  double  Eint, Tint ;
  int	  *listevoisin ;
} ;
typedef struct part Particules ;


/* * * * * structure etat thermodynamique */
struct etat {
  int     nat ;
  double  volume, temperature, pression ;
  double  massetot, mvol ;
  double  virielx, viriely, virielz ;
  double  etot, ektot, eptot ;
  double  vgx, vgy, vgz ;
  
  /* moyennes */
  double  ave_temp, sig_temp ;
  double  ave_pres, sig_pres ;
  double  ave_etot, sig_etot ;
  double  ave_ektot, sig_ektot ;
  double  ave_eptot, sig_eptot ;
  
  /* DPDE */
  double  eitot ;
  double  ave_eitot, sig_eitot ;
  double  ave_temp_int, sig_temp_int ;
} ;
typedef struct etat Etat ;


/* * * * * structure contenant les donnees */
struct data {
  /* variables de la namelist = NFLAG */
  char    methode[ LEN ] ;		// type de calcul
  int     nstep ;			// nbre iteration
  double  dt ;				// pas de temps
  double  xsi ;				// friction
  double  temp0 ;			// temperature cible
  double  rcut ;			// cut off
  double  rverlet ;			// cut off liste voisin
  int     graine ;              	// graine nbre aleatoire
  char    aname[LEN] ;			// nom de l'atome  
  double  masse ;			// masse de l'atome
  double  LJsigma ;			// sigma Lenard Jones
  double  LJeps ;			// epsilon Lenard Jones
  char    cristal[ LEN ] ;		// type de maille
  double  a0 ;				// parametre de maille
  int     Nx, Ny, Nz ;			// nbre de maille dans chaque direction
  int     nbrevoisinmax ;		// dimension liste voisins
  int     nave, ncrd, nout, nvel ; 	// frequence des sorties
  double  pas_radial_dist ;		// pas du g(r)
  char    restart[LEN] ;		// nom du fichier restart
  double  vginitx, vginity, vginitz ; 	// vitesse globale initiale
  double  capacite_calo ;		// capacite calorifique
  double  temp_interne0 ;		// temperature interne cible initiale
  int     freqTint ;			// frequence impression temperature interne
  double  sigmaDPDE ;			// controle la friction dans DPDE

  /* constantes */
  double  boltzman ;
  double  avogadro ;
  double  qelectron ;
  double  aBohr ;
  double  Ang ;
  double  pi ;
  
  /* variables supplementaires */
  long    idum ;		// graine long
  double  boxx, boxy, boxz ;	// dimension du systeme
  double  LJsigma6 ;		// sigma^6
  double  LJ4eps ;		// 4*epsilon 
  double  LJshift ;		// potentiel(rcut)
  double  rcut2 ;		// rcut^2
  double  rverlet2 ;		// rverlet^2
  double  dmax2 ;		// (rcut-rverlet)^2
  int     numerostep ;		// numero iteration
  int     majvoisin ;		// flag mise a jour des voisins
  int     nbremajvoisin ;	// nbre de mise a jour des voisins
  double  *radial_dist ;	// distribution g(r)
  int     npas_radial_dist ;	// nbre de bin de g(r)
  clock_t start, end, t_init ;	// temps
  
  /* nom des fichiers entree/sortie */
  char amdout[LEN] ;
  char amdcrd[LEN] ;
  char amdvel[LEN] ;
  char acntrl[LEN] ;
  char aradial_dist[LEN] ;
  char arestart[LEN] ;
  char atemp_int[LEN] ;
  char input[LEN] ;

  /* flux des fichiers de sortie recurent */
  FILE *mdout ;
  FILE *cntrl ;
  FILE *ftemp_int ;
} ;
typedef struct data Data ;


