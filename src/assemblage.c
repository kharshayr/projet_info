#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <liste.h>
#include <assemblage.h>
#include <symb.h>
#include <verif.h>

void affiche_assembl(void* e) {
  printf("%2i ", ((assembl*)e)->num_ligne);
  if(((assembl*)e)->empty){
    printf("\t\t     %s\n",((assembl*)e)->ligne);
  }
  else {
    printf("%08x %08x %s\n", ((assembl*)e)->decalage, ((assembl*)e)->code_ligne, ((assembl*)e)->ligne);
  }
}

char* reg_mnemo2[] = {"$zero", "$0", "$at", "$1", "$v0", "$2", "$v1", "$3", "$a0", "$4", "$a1", "$5", "$a2", "$6", "$a3", "$7",
 "$t0", "$8", "$t1", "$9", "$t2", "$10", "$t3", "$11", "$t4", "$12", "$t5", "$13", "$t6", "$14", "$t7", "$15", "$s0",
  "$16", "$s1", "$17", "$s2", "$18", "$s3", "$19", "$s4", "$20", "$s5", "$21", "$s6", "$22", "$s7", "$23", "$t8",
  "$24", "$t9", "$25", "$k0", "$26", "$k1", "$27", "$gp", "$28", "$sp", "$29", "$fp", "$30", "$ra", "$31"};

int valeur_registre(char* mot, char** tab) {
  int i=0;
  while (i<64){
		if(strcmp(tab[i],mot)==0){
			return floor(i/2);
    }
		i++;
  }
  printf("registre inexistant (assemblage.c ligne23)");
  return 0;
}

assembl* init_assembl(char* line, int line_num) {
  assembl* a = calloc(1,sizeof(*a));
  if(a==NULL) {printf("Erreur allocation ligne d'assemblage"); return NULL;}
  a->ligne = calloc(1,sizeof(char)*strlen(line));
  strcpy(a->ligne,line);
  a->num_ligne = line_num;
  a->empty = 1;
  return a;
}

Liste chercher_ligne_col(int num_ligne, Liste col){
  if(liste_vide(col)) return NULL;
  Liste p_col = col;
  if(((instruction*)p_col->val)->inst->lex.nl==num_ligne){
    return p_col;
  }
  while(!liste_vide(p_col->suiv)) {
    if(((instruction*)p_col->val)->inst->lex.nl==num_ligne){
      return p_col;
    }
    p_col=p_col->suiv;
  }
  return NULL;
}

Liste chercher_ligne_bss(int num_ligne, Liste col){
  if(liste_vide(col)) return NULL;
  Liste p_col = col;
  if(((symb*)p_col->val)->lex.nl==num_ligne){
    return p_col;
  }
  while(!liste_vide(p_col->suiv)) {
    printf("%i",((symb*)p_col->val)->lex.nl);
    if(((symb*)p_col->val)->lex.nl==num_ligne){
      return p_col;
    }
    p_col=p_col->suiv;
  }
  return NULL;
}

Liste chercher_ligne_ass(int num_ligne, Liste* ass){
  if(liste_vide(*ass)) return NULL;
  Liste p_ass = *ass;
  if(((assembl*)p_ass->val)->num_ligne==num_ligne){
    return p_ass;
  }
  while(!liste_vide(p_ass->suiv)) {
    if(((assembl*)p_ass->val)->num_ligne==num_ligne){
      return p_ass;
    }
    p_ass=p_ass->suiv;
  }
  return NULL;
}

void calcul_code_assemblage(Liste col_text, Liste col_data, Liste col_bss, Liste* assembl_l, unsigned int n_lines, symb* symb_t) {
  int i, j, num_op, pseudo;
  Liste temp;
  Liste temp_ass;
  unsigned long code;
  symb* etiqu;

  for(i=0;i<n_lines;i++) {

    temp = chercher_ligne_col(i,col_text);

    if(!liste_vide(temp)) {

      temp_ass = chercher_ligne_ass(i,assembl_l);
      if(!liste_vide(temp->suiv) && ((instruction*)temp->suiv->val)->inst->lex.nl == i) pseudo=1;
      else pseudo = 0;
      
      for(j=0;j<pseudo+1;j++) {
        if(j == 1) {
          assembl* new_ass = init_assembl("", i);
          Liste next = temp_ass->suiv;
          temp_ass->suiv=calloc(1,sizeof(*(temp->suiv)));
          if(temp_ass==NULL) ERROR_MSG("Erreur allocation element assemblage pour pseudo assemblage.c l.117");
          temp_ass->suiv->val=(void*)new_ass;
          temp_ass->suiv->suiv=next;
          temp=temp->suiv;
        }

        for(num_op=0;num_op<((instruction*)temp->val)->inst_def.nb_op;num_op++) {

          switch(((instruction*)temp->val)->Operande[num_op].ope_typ){
            case ETI:
              etiqu = rech_mot_symb(((instruction*)temp->val)->Operande[num_op].ope_val->eti,symb_t);
              if(!strcmp(((instruction*)temp->val)->inst->lex.tok,"LW")) {
                code = (unsigned long) 0x0000FFFF && (unsigned long) etiqu->deca;
              }
              if(!strcmp(((instruction*)temp->val)->inst->lex.tok,"LUI")) {
                code = (unsigned long) 0xFFFF0000 && (unsigned long) etiqu->deca;
              }
            break;
            case REG:
              code = (unsigned long) valeur_registre(((instruction*)temp->val)->Operande[num_op].ope_val->reg,reg_mnemo2);
            break;
            case SA:
              code = (unsigned long) ((instruction*)temp->val)->Operande[num_op].ope_val->sa;
            break;
            case ABS:
            code = (unsigned long) ((instruction*)temp->val)->Operande[num_op].ope_val->abs;
            break;
            case IMD:
            code = 0xFFFF & (unsigned long) (((instruction*)temp->val)->Operande[num_op].ope_val->imd);
            break;
            case REL:
            code = 0xFFFF & (unsigned long) (((instruction*)temp->val)->Operande[num_op].ope_val->rel);
            break;
            default:
              ERROR_MSG("Mauvais type operande pour le calcul du code assemblage");
            break;
          }
          code <<= ((instruction*)temp->val)->inst_def.poidsBits[num_op]; /* decalage code operande par son poids */
          ((assembl*)temp_ass->val)->code_ligne |= code | ((instruction*)temp->val)->inst_def.codeBinaire; /* ET entre masque définition et code */
        }
        ((assembl*)temp_ass->val)->decalage = ((instruction*)temp->val)->inst->deca;
        ((assembl*)temp_ass->val)->empty = 0;
      }
    }

    temp = chercher_ligne_col(i,col_data);
    if(!liste_vide(temp)){

    }

    temp = chercher_ligne_bss(i,col_bss);
    if(liste_vide(temp)){

    }
    else {

    }

  }
  return ;
}
