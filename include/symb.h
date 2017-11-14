#ifndef _SYMB_H_
#define _SYMB_H_

#define N 100

typedef enum sec{
	bss, data, text, INI,} sect;

typedef struct sym{
	lexeme lex;                             /* Lexeme observé */
	sect section;                           /* Section dans laquelle on est*/
	int deca;} symb;                        /* Décalage en octets*/

void ajout_liste(Liste* l, Liste p,int t,int* d);

void affiche_col(void *);

void affiche_tab(symb* s);

int hachage(char* mot);

void ajout_tab(symb* t, symb* s);

void init_symb(symb* s);

Liste* init_liste();

void tabl_symb(Liste l, symb* s, Liste* data_l, Liste* text, Liste* bss);

#endif
