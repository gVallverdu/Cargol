
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* Les 26 translations pour les conditions periodiques                       */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define	NIMG	27

/* condition periodiques */
int	transx[ NIMG ] ;
int	transy[ NIMG ] ;
int	transz[ NIMG ] ;

int init_translation( void ) {

	int	erreur = 0 ;

	// liste des 26 translations entières possibles

	transx[ 0] =  0  ; transy[ 0] =  0  ; transz[ 0] =  0  ;

	transx[ 1] =  1  ; transy[ 1] =  0  ; transz[ 1] =  0  ;
	transx[ 2] =  0  ; transy[ 2] =  1  ; transz[ 2] =  0  ;
	transx[ 4] =  0  ; transy[ 3] =  0  ; transz[ 3] =  1  ;
	transx[ 4] =  1  ; transy[ 4] =  1  ; transz[ 4] =  0  ;
	transx[ 5] =  1  ; transy[ 5] =  0  ; transz[ 5] =  1  ;
	transx[ 6] =  0  ; transy[ 6] =  1  ; transz[ 6] =  1  ;
	transx[ 7] =  1  ; transy[ 7] =  1  ; transz[ 7] =  1  ;

	transx[ 8] = -1  ; transy[ 8] =  0  ; transz[ 8] =  0  ;
	transx[ 9] =  0  ; transy[ 9] = -1  ; transz[ 9] =  0  ;
	transx[10] =  0  ; transy[10] =  0  ; transz[10] = -1  ;
	transx[11] = -1  ; transy[11] = -1  ; transz[11] =  0  ;
	transx[12] = -1  ; transy[12] =  0  ; transz[12] = -1  ;
	transx[13] =  0  ; transy[13] = -1  ; transz[13] = -1  ;
	transx[14] = -1  ; transy[14] = -1  ; transz[14] = -1  ;

	transx[15] =  1  ; transy[15] = -1  ; transz[15] =  0  ;
	transx[16] = -1  ; transy[16] =  1  ; transz[16] =  0  ;
	transx[17] =  0  ; transy[17] = -1  ; transz[17] =  1  ;
	transx[18] =  0  ; transy[18] =  1  ; transz[18] = -1  ;
	transx[19] =  1  ; transy[19] =  0  ; transz[19] = -1  ;
	transx[20] = -1  ; transy[20] =  0  ; transz[20] =  1  ;

	transx[21] =  1  ; transy[21] =  1  ; transz[21] = -1  ;
	transx[22] =  1  ; transy[22] = -1  ; transz[22] =  1  ;
	transx[23] = -1  ; transy[23] =  1  ; transz[23] =  1  ;
	transx[24] = -1  ; transy[24] = -1  ; transz[24] =  1  ;
	transx[25] = -1  ; transy[25] =  1  ; transz[25] = -1  ;
	transx[26] =  1  ; transy[26] = -1  ; transz[26] = -1  ;

	return erreur ;
}
