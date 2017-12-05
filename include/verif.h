#ifndef _VERIF_H_
#define _VERIF_H_

#include <stdio.h>

typedef enum opetyp_e {
  REG, IMD, REL, SA, ABS, OFB, ERR_TYP_OP ,ETI
} opetyp;

typedef union opeval_u {
  char* reg;
  short imd;
  short rel;
  char* eti;
  unsigned short sa;
  unsigned int abs;
  char* ofb;
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

symb* rech_mot(char*,symb*);
Liste verif_arg_text(Liste*, inst_def_t *, int,symb*);
int recherche_instr(char*, inst_def_t *, int);

#endif
