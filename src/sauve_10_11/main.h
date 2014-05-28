/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* fichier main.h                                                            */
/*                                                                           */
/* contient les includes necessaires                                         */
/* contient les definitions de parametres                                    */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// librairies
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

// dimensions
#define	LEN	100
#define	NFLAG	24

// id methodes
#define idNVE		1
#define	idlangevin	2
#define	idLNVT		3
#define	idDPD		4
#define	idDPDE		5

// choix generateur des gaussiennes
// #define gaussienne	gauss_G
#define gaussienne	gasdev
