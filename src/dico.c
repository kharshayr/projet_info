#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dico.h>

inst_def_t * lect_dico_int(char* nomFichierDico, int* p_nb_inst) {
 FILE *f1;
 int i;
 char s1[512];
 inst_def_t * tab;
 f1=fopen(nomFichierDico,"r");
 if (f1==NULL) {
   WARNING_MSG("Erreur ouverture fichier dictionnaire");
   return NULL;
 }
 if( fscanf(f1,"%d", p_nb_inst) != 1){
   WARNING_MSG("Erreur nombre d'instructions dans le dictionnaire");
   return NULL;
 }
 tab=calloc(*p_nb_inst,sizeof(*tab));
  if(tab == NULL) ERROR_MSG("Erreur allocation tableau dictionnaire. Arret du programme");
 for (i=0; i<*p_nb_inst; i++) {
   if(fscanf(f1,"%s %c %d",s1,&(tab[i].type),&(tab[i].nb_op)) != 3) {
     WARNING_MSG("Erreur définition instruction dictionnaire");
     free(tab);
     return NULL;
   }
   tab[i].symbole=strdup(s1);
 }
 fclose(f1);
 return tab;
}
