
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           * 
 * message d'accueil                                                         * 
 * parce qu'il fallait bien une routine qui soit pas serieuse ;)             *
 *                                                                           * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main.h"

int message1( void ) {

  int erreur = EXIT_SUCCESS ;
  
  printf("\n\n\t  _____                        _          \n") ;
  printf(    "\t / ____|                      | |         \n") ;
  printf(    "\t| |      __ _ _ __  ____  ___ | |         \n") ;
  printf(    "\t| |     / _` | '__|/ _  |/ _ \\| |        \n") ;
  printf(    "\t| |____| (_| | |  | (_| | (_) | |         \n") ;
  printf(    "\t \\_____|\\__,_|_|   \\___ |\\___/|_|     \n") ;
  printf(    "\t                    __/ |                 \n") ;
  printf(    "\t                   |___/                  \n\n") ;
  
  return erreur ;
}


int message2( Data *d ) {

  int erreur = EXIT_SUCCESS ;
  FILE *cntrl = d->cntrl ;

  fprintf(cntrl,"\n  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n" );
  fprintf(cntrl,  "  *                                                             *\n" ); 
  fprintf(cntrl,  "  *                         cargol                              *\n" );
  fprintf(cntrl,  "  *                                                             *\n" ); 
  fprintf(cntrl,  "  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n\n" );

  fprintf(cntrl,  "                              °\\____       ________             \n" ); 
  fprintf(cntrl,  "                               /-   \\     /  ____  \\           \n" ); 
  fprintf(cntrl,  "                               \\_    \\   /  /    \\  \\     -- \n" ); 
  fprintf(cntrl,  "                                 \\    \\ (   \\__/  )  )    --  \n" ); 
  fprintf(cntrl,  "                                  \\    \\_\\ \\_____/  /__    --\n" ); 
  fprintf(cntrl,  "                                   \\______\\________/___'=-   \n\n\n\n");

  return erreur ;
}


int message_help( void ) {

  int erreur = EXIT_SUCCESS ;

  printf("\n" ) ;
  printf("  Syntaxe simple       : cargol fichier_input \n" ) ;
  printf("  \t Exemple : ./cargol input_NVE \n\n" ) ;
  printf("  Syntaxe avec options : cargol <option1> <option2> ... \n") ;
  printf("  \t <optionX> : -flag mot  \n" ) ;
  printf("  \t           : mot designe un nom de fichier d'entree/sortie associe a une option \n" ) ;
  printf("  \t Exemple   : -i input_NVE \n\n" ) ;
  printf("  Liste des options : \n") ;
  printf("\t-i : nom du fichier input (entree) \n" ) ;
  printf("\t\t pas de defaut \n\n" );
  printf("\t-d : nom du fichier de sortie (sortie) \n" ) ;
  printf("\t\t defaut mdout.dat \n\n" );
  printf("\t-o : nom du fichier de controle des donnees (sortie) \n" ) ;
  printf("\t\t defaut md_control \n\n" );
  printf("\t-x : nom du fichier contenant les coordonnees (sortie) \n" ) ;
  printf("\t\t defaut mdcrd.xyz \n\n" );
  printf("\t-v : nom du fichier contenant les vitesses (sortie) \n" ) ;
  printf("\t\t defaut mdvel.xyz \n\n" );
  printf("\t-g : nom du fichier de la distribution radiale (sortie) \n" ) ;
  printf("\t\t defaut radial_dist.dat \n\n" );
  printf("\t-r : nom du fichier contenant la configuration finale (sortie) \n" ) ;
  printf("\t\t defaut restart \n\n" );
  printf("\t-t : nom du fichier des temperatures internes (sortie) \n" ) ;
  printf("\t\t defaut temp_interne.dat \n\n\n" );
  
  printf("  Exemple complet : \n" );
  printf(" ./cargol -i input -d mdout.dat -o md_control -x mdcrd -v mdvel -g gdeR.dat\n") ;
  printf("-r restart -t temp_interne.dat\n\n" ) ;
  
  printf("  L'option -i est obligatoire, toutes les autres sont facultatives\n") ;
  printf("  Attention : Les fichiers de sortie sont ecrase s'il existait deja.\n\n") ;
  
  exit(EXIT_SUCCESS);
  
  return erreur ;
}

