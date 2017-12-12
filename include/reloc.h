#ifndef _RELOC_H_
#define _RELOC_H_

#include <stdio.h>
#include <verif.h>
#include <symb.h>

typedef enum reltyp_e {
  R_MIPS_32, R_MIPS_26, R_MIPS_LO16, R_MIPS_HI16, UNDEFINED,
} reltyp;

typedef struct reloc_s {
  int rel_addresse;
  symb* etiquette;
  reltyp rel_type;
} reloc;

void affiche_reloc(void*);
reloc* init_entree_reloc(instruction*, int, symb*);
Liste creer_table_reloc(Liste, symb*);

#endif
