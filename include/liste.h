#ifndef _LISTE_H_
#define _LISTE_H_

#include <stdio.h>

typedef void(*p_affiche)(void*);

typedef struct Liste_s {
  struct Liste_s * suiv;
  void* val;
} * Liste;

void affiche_erreurs_lex(Liste l);
Liste creer_liste(void);
int liste_vide(Liste );
void visualiser_liste(Liste ,p_affiche);
Liste ajout_tete(void* , Liste );
Liste supprimer_tete(Liste ) ;
Liste ajout_queue(void* , Liste );
Liste supprimen(int , Liste );

#endif
