#include <stdio.h>
#include <stdlib.h>
#include <liste.h>
#include <lex.h>

Liste creer_liste(void) {
	return NULL;
}

int liste_vide(Liste l) {
	return !l;
}

void visualiser_liste (Liste l, p_affiche aff) {
	Liste p;
	p=l;
	while (!liste_vide(p)) {
		(*aff)(p->val);	/* fonction d'affichage propre à l'élément de la liste, appelée par son pointeur */
		p=p->suiv;
	}
}

Liste ajout_tete(void* c, Liste l) {
	Liste p=calloc(1,sizeof(*p));
	if (p==NULL) return NULL;
	p->val=c;
	p->suiv=l;
	return p;
}

Liste supprimer_tete(Liste l) {
	if (!liste_vide(l)) {Liste p;
		p=l->suiv;
		free(l);
		return p;
	}
	else return NULL;
}

Liste ajout_queue(void* c, Liste l){
	Liste p=l;
	if(liste_vide(l)) {l=calloc(1,sizeof(*l));l->val=c;return l;}
	while (!liste_vide(p->suiv)) p=p->suiv;
	p->suiv=calloc(1,sizeof(*p));
	(p->suiv)->val=c;
	return l;
}

Liste supprimen(int n, Liste l) {
	Liste p;
	p=l;
	int i=1;
	if(n==1) {free(l); return p->suiv;}
	for(i=1; i<n-1; i++) {
		if (!liste_vide(p->suiv)) return l;
		p=p->suiv;
	}
	if (!liste_vide(p->suiv)) return l;
	Liste q=p->suiv;
	free(p->suiv);
	p->suiv=q->suiv;
	return l;
}
