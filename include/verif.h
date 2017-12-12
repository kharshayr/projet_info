#ifndef _VERIF_H_
#define _VERIF_H_

#include <stdio.h>
#include <symb.h>
#include <dico.h>

typedef enum opetyp_e {
  REG, IMD, REL, SA, ABS, OFB, ERR_TYP_OP ,ETI, WRD, CHN
} opetyp;

typedef union opeval_u {
  char* reg;
  short imd;
  short rel;
  char* eti;
  unsigned short sa;
  unsigned int abs;
  char* ofb; /* "chaine de C pour  */
  unsigned long wrd;
  char* chaine;
} opeval;

typedef struct opestruct_s {
  opetyp ope_typ;
  opeval* ope_val;
} opestruct;

typedef struct instruction_s {
  symb* inst;
  inst_def_t inst_def;
  opestruct* Operande;
    /*  opetyp ope_typ;
        opeval ope_val; */
} instruction;

void affiche_liste_ope_data(Liste);
void affiche_liste_ope_text(Liste);
char* rech_mot(char*, char**);
symb* rech_mot_symb(char*,symb*);
Liste verif_arg_data(Liste*);
Liste verif_arg_text(Liste*, inst_def_t *, int,symb*);
int recherche_instr(char*, inst_def_t *, int);

#endif
