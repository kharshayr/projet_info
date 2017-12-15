#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include <reloc.h>
#include <lex.h>
#include <symb.h>
#include <verif.h>
#include <liste.h>
#include <dico.h>

void affiche_reloc(void* e) {
    static char* mesval[] = {"R_MIPS_32", "R_MIPS_26", "R_MIPS_LO16", "R_MIPS_HI16"};
    static char* mesval2[] = {".bss", ".data", ".text", " ", " "};
    if(((reloc*)e)->etiquette->section == undefined) {
      printf("%08x\t%s\t[UNDEFINED]\t%s\n",((reloc*)e)->rel_addresse,mesval[((reloc*)e)->rel_type],((reloc*)e)->etiquette->lex.tok);
    }
    else {
      printf("%08x\t%s\t%s:%08x\t%s\n",((reloc*)e)->rel_addresse,mesval[((reloc*)e)->rel_type],mesval2[((reloc*)e)->etiquette->section],((reloc*)e)->etiquette->deca,((reloc*)e)->etiquette->lex.tok);
    }
}

/*void affiche_reloc(void* e) {
    static char* mesval[] = {"R_MIPS_32", "R_MIPS_26", "R_MIPS_LO16", "R_MIPS_HI16"};
    static char* mesval2[] = {".bss", ".data", ".text", " ", " "};
    if(((reloc*)e)->etiquette->section == undefined) {
      printf("%08x\t%s\t[UNDEFINED]\t%s\n",((reloc*)e)->rel_addresse,mesval[((reloc*)e)->rel_type],((reloc*)e)->etiquette->lex.tok);
    }
    else {
      printf("%08x\t%s\t%s:%08x\t%s\n",((reloc*)e)->rel_addresse,mesval[((reloc*)e)->rel_type],mesval2[((reloc*)e)->etiquette->section],((reloc*)e)->etiquette->deca,((reloc*)e)->etiquette->lex.tok);
    }
}*/

reloc* init_entree_reloc(instruction* i, int num_op, symb* symb_t) {
    reloc* r = calloc(1,sizeof(reloc));
    if(r==NULL) { printf("Erreur allocation entrée de relocation"); return NULL; }

    r->etiquette = rech_mot_symb(i->Operande[num_op].ope_val->eti,symb_t);

    r->rel_addresse = i->inst->deca;
    char* typOp = i->inst_def.arg;

    if(!strcmp(typOp,"ABS")) {
        r->rel_type = R_MIPS_26;
        i->Operande[num_op].ope_val->abs = (r->etiquette->deca)>>2;
        i->Operande[num_op].ope_typ = ABS;
    }

    else if(!strcmp(typOp,"REL")) {
        i->Operande[num_op].ope_val->rel = ((r->etiquette->deca) - (i->inst->deca) -4)>>2;
        i->Operande[num_op].ope_typ=REL;
        return NULL;
    }

    else if(!strcmp(typOp,"I") || !strcmp(typOp,"OFB")) {
        if(strcmp(i->inst->lex.tok, "LUI") == 0) {r->rel_type = R_MIPS_HI16;}
        else {r->rel_type = R_MIPS_LO16;}
    }

    else if(!strcmp(typOp,"WRD")) {
        r->rel_type = R_MIPS_32;
    }

    else {
        printf("Erreur relocation mauvaise définition : %s\n", i->inst->lex.tok);
        return NULL;
    }
    return r;
}

Liste creer_table_reloc(Liste inst_l, symb* symb_t) {
  Liste pl=inst_l;
  Liste reloc_l=NULL;
  int i=0;
  while(!liste_vide(pl)) {
    int j=0;
    for(j=0;j<(((instruction*)pl->val)->inst_def.nb_op);j++) {
      if(((instruction*)pl->val)->Operande[j].ope_typ==ETI) {
        reloc* temp=init_entree_reloc(pl->val, j, symb_t);
        if(NULL!=temp) {
          reloc_l = ajout_queue((void*) temp, reloc_l);
          i++;
        }
      }
    }
    pl=pl->suiv;
  }
  return reloc_l;
}
