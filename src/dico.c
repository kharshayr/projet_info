#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <lex.h>
#include <liste.h>
#include <symb.h>
#include <dico.h>

void affiche_erreurs_dico(Liste l){
      printf("Erreur(s) d'instructions :\n");
      while (!liste_vide(l)) {
      	if (((symb*)(l->val))->lex.typ==ERREUR) affiche(l->val);
	    l=l->suiv;}
}

void strtoupper(char* str){
	int i;
	for (i=0;i<strlen(str);i++){
        	str[i] = (char)toupper((int)str[i]);}}

inst_def_t * lect_dico_int(char* nomFichierDico, int* p_nb_inst) {
 FILE *f1;
 int i,j;
 char* s1=malloc(512);
 char* s2=malloc(512);
 char bs[32];
 inst_def_t * tab;
 f1=fopen(nomFichierDico,"r");

 if (f1==NULL) {
   WARNING_MSG("Erreur ouverture fichier dictionnaire");
   return NULL;}
 if( fscanf(f1,"%d", p_nb_inst) != 1){
   WARNING_MSG("Erreur nombre d'instructions dans le dictionnaire");
   return NULL;}
 tab=calloc(*p_nb_inst,sizeof(*tab));
  if(tab == NULL) ERROR_MSG("Erreur allocation tableau dictionnaire. Arret du programme");
 for (i=0; i<*p_nb_inst; i++) {

   if(fscanf(f1,"%s %c %d %s",s1,&(tab[i].type),&(tab[i].nb_op),s2) != 4) {
     WARNING_MSG("Erreur définition instruction dictionnaire");
     free(tab);
     return NULL;
   }

   switch(tab[i].type) {
     case 'R':
     fscanf(f1, "%s %s %s %s %s %s", &bs[0], &bs[6], &bs[11], &bs[16], &bs[21], &bs[26]);
     tab[i].codeBinaire=strtol(bs,NULL,2);
     break;
     case 'I':
     fscanf(f1, "%s %s %s %s", &bs[0], &bs[6], &bs[11], &bs[16]);
     tab[i].codeBinaire=strtol(bs,NULL,2);
     break;
     case 'J':
     fscanf(f1, "%s %s", &bs[0], &bs[6]);
     tab[i].codeBinaire=strtol(bs,NULL,2);
     break;
   }

   int* tab_poids = calloc(tab[i].nb_op,sizeof(int));
   for(j=0;j<tab[i].nb_op;j++) {
     fscanf(f1, "%i", tab_poids+j);
   }
   tab[i].poidsBits=tab_poids;

   tab[i].symbole=s1;
   tab[i].arg=s2;
   s1=malloc(512);
   s2=malloc(512);
 }
 fclose(f1);
 return tab;
}
