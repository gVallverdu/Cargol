
#include <stdio.h>
#include <stdlib.h>

double random_number(void) {
  return (double) rand() / RAND_MAX ;
}

void main( void) {
  
  int k, nvois, ivois, max, min, c, i, dejavu ;
  int liste[200] ;
  
  for ( k=0 ; k<200 ; k++ ) liste[k] = -1 ;

  max = -1 ;
  min = 1000000 ;
  nvois = 99 ;
  k = 0 ;
  c = 0 ;
  while ( k != nvois ) {

    ivois = (int) ( nvois * random_number() ) ;

    if ( ivois > max ) max = ivois ;
    if ( ivois < min ) min = ivois ;

    dejavu = 0 ;
    for ( i = 0 ; i < k ; i++ ) {
      if ( liste[i] == ivois ) dejavu++ ;
    }
    
    if ( dejavu == 0 ) {
      liste[k] = ivois ;
      printf("k =%3d c =%3d\n", k ,c ) ;
      k++ ;
      
    } else if ( dejavu > 1 ) {
      printf("\n\non a enregistre plusieurs fois le meme\n") ;
      exit(0) ;      
    }
    c++ ;
  }
  
  printf("max %d \t min %d \n", max, min );
  printf("nbre de tirage : %d\n", c ) ;
  for ( k = 0 ; k < nvois ; k++ )printf("%3d %3d\n", k, liste[k] ) ;
  
}
