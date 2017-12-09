#ifndef _SYMB_H_
#define _SYMB_H_

#define N 100

#include<stdlib.h>
#include<string.h>

#include<lex.h>
#include<symb.h>
#include<dico.h>


typedef enum sec{
	bss, data, text, INI,} sect;

typedef struct sym{
	lexeme lex;                             /* Lexeme observé */
	sect section;                           /* Section dans laquelle on est*/
	int deca;} symb;                        /* Décalage en octets*/

void ajout_liste(Liste*, Liste, int, int*);
void affiche_col(void*);
void affiche_tab(symb*);
void affiche_erreurs_gram(Liste);
int hachage(char*);
void ajout_tab(symb*, symb*);
void init_symb(symb*);
Liste* init_liste();
void tabl_symb(Liste, symb*, Liste*, Liste*, Liste*);
void pseudo_instr(Liste, int);

#endif
