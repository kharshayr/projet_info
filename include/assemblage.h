#ifndef _ASSEMBLAGE_H_
#define _ASSEMBLAGE_H_

#include <stdio.h>
#include <notify.h>
#include <symb.h>

typedef struct assembl_s {
  int num_ligne;
  int decalage;
  int code_ligne;
  char* ligne;
  int typ_aff;
  /*
  0 : tout afficher
  1 :
  2 :
  ..
  8 :
  9 : code non visible
  10 : .. pour .space 
  */
} assembl;

void affiche_assembl(void* );
int valeur_registre(char*, char**);
assembl* init_assembl(char*, int);
Liste chercher_ligne_col(int , Liste );
Liste chercher_ligne_bss(int , Liste );
Liste chercher_ligne_ass(int , Liste* );
void calcul_code_assemblage(Liste , Liste , Liste , Liste* , unsigned int* , symb*);


#endif
