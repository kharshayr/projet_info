/**
 * @file lex.h
 * @author François Portet <francois.portet@imag.fr>
 * @brief Lexem-related stuff.
 *
 * Contains lexem types definitions, some low-level syntax error codes,
 * the lexem structure definition and the associated prototypes.
 */

#ifndef _LEX_H_
#define _LEX_H_

#include <stdio.h>
#include <liste.h>

typedef enum typelex_s {
  COMMENT, NL, SYMBOLE, DEUX_PTS, VIRGULE, DEB_OFFSETBASE, OFFSETBASE, DIRECTIVE, REGISTRE, DECIMAL, HEXA, ZERO, CHAINE, INIT, ERREUR,
} typelex;

typedef struct lexeme_s {
  char* tok; /* Le token */
  typelex typ; /* Le type du token */
  int nl; /* Le numéro de ligne du token dans le fichier */
} lexeme;

void affiche(void* );
void affiche_erreurs_lex(Liste l);
lexeme*	init_lex(char*, int );
void	lex_fsm(lexeme* );
void	lex_standardise( char*, char*  );
Liste	lex_read_line( char *, int, Liste);
Liste	lex_load_file( char *, unsigned int *, Liste , Liste*);


#endif /* _LEX_H_ */
