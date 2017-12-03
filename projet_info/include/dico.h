#ifndef _DICO_H_
#define _DICO_H_

#include <stdio.h>
#include <notify.h>

typedef struct { char * symbole; char type ; int nb_op ;char* arg ;} inst_def_t;

void strtoupper(char*);
void affiche_erreurs_dico(Liste l);
inst_def_t* lect_dico_int(char*, int*);

#endif /* _DICO_H_ */
