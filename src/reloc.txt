#include <stdlib.h>
#include <stdio.h>

#include <reloc.h>
#include <lex.h>
#include <symb.h>
#include <verif.h>
#include <liste.h>

reloc* init_entree_reloc_text(instruction* i, int num_op, symb* symb_t) {
    reloc* r = calloc(1,sizeof(reloc*));
    if(r==NULL) { printf("Erreur allocation entrée de relocation"); return NULL; }
    r->etiquette = i->Operande[num_op]->ope_val.eti;

    symb* etiq_p = rech_mot(i->Operande[num_op]->ope_val.eti,symb_t);
    if (etiq_p == NULL) {
      r->rel_typ = UNKNOWN;
      r->rel_addresse = 0;
      return r;
    }

    int d = i->inst->deca;
    switch (i->ope_typ->inst_def->typeop[num_op]) { /*//////////////////////////////////////////////////////////////*/

      case ABS:
        r->rel_type = R_MIPS_26;
        r->rel_adresse = d;
        break;

      case REL:
        i->Operande[num_op]->ope_val.rel= (d+4 - e->deca)>>2;
        i->Operande[num_op]->ope_typ=REL;
        return NULL;
        break;

      case IMD:
        if(strcmp(, "lui" == 0)) {r->rel_type = R_MIPS_HI16;} /*//////////////////////////////////////////////////////////////////////////////*/
        else {r->rel_type = R_MIPS_LO16;}
        break;
      default :
        printf("Erreur relocation mauvaise définition");
        return NULL;
        break;
    }
    return r;
}

reloc* creer_table_reloc_text(Liste inst_l, symb* symb_t) {
  Liste pl=inst_l;
  reloc* reloc_t=NULL;
  int i=0;
  while(!liste_vide(pl)) {
    int j=0;
    for(j=0;j<pl->val->inst_def>nb_op;j++) {
      if(pl->val->Operande[j]->ope_typ==ETI) {
        reloc* temp=init_entree_reloc_text(pl->(instruction*)val, j, symb_t);
        if(NULL!=temp) {
          (reloc_t+i)=temp;
          i++;
        }
      }
    }
    pl=pl->suiv;
  }
  return reloc_t;
}

reloc* init_entree_reloc_data(symb* data_l, int num_op, symb* symb_t) {
    reloc* r = calloc(1,sizeof(reloc*));
    if(r==NULL) { printf("Erreur allocation entrée de relocation"); return NULL; }
    r->etiquette = data_l;

    symb* etiq_p = rech_mot(data_l->lex->tok,symb_t);
    if (etiq_p == NULL) {
      r->rel_typ = UNKNOWN;
      r->rel_addresse = 0;
      return r;
    }

    r->rel_type = R_MIPS_32;
    r->rel_adresse = data_l->deca;
}

reloc* creer_table_reloc_data(Liste data_l, symb* symb_t) {
  Liste pl=data_l;
  reloc* reloc_t=NULL;
  int i=0;
  while(!liste_vide(pl)) {
    if(==ETI) { /*//////////////////////////////////////////////////////////////////////////////*/
      reloc* temp=init_entree_reloc_data(pl->(instruction*)val, j, symb_t);
      if(NULL!=temp) {
        (reloc_t+i)=temp;
      }
    pl=pl->suiv;
  }
