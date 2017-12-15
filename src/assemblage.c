#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <liste.h>
#include <assemblage.h>
#include <symb.h>
#include <verif.h>

void affiche_assembl(void* e) {
  printf("%3i ", ((assembl*)e)->num_ligne);
  if(((assembl*)e)->typ_aff==9){
    printf("\t\t     %s\n",((assembl*)e)->ligne);
  }
  else if(((assembl*)e)->typ_aff == 0){
    printf("%08X %08X %s\n", ((assembl*)e)->decalage, ((assembl*)e)->code_ligne, ((assembl*)e)->ligne);
  }
  else if(((assembl*)e)->typ_aff == 10){
    printf("%08X 0000...  %s\n", ((assembl*)e)->decalage, ((assembl*)e)->ligne);
  }
  else if(((assembl*)e)->typ_aff == 1){
    printf("%08X %s\n", ((assembl*)e)->decalage, ((assembl*)e)->ligne);
  }
  else if(((assembl*)e)->typ_aff == 2){
    printf("%08X %2X %s\n", ((assembl*)e)->decalage, ((assembl*)e)->code_ligne >> (4*(8- ((assembl*)e)->typ_aff)), ((assembl*)e)->ligne);
  }
  else if(((assembl*)e)->typ_aff == 3){
    printf("%08X %02X       %s\n", ((assembl*)e)->decalage, ((assembl*)e)->code_ligne, ((assembl*)e)->ligne);
  }
  else if(((assembl*)e)->typ_aff == 4){
    printf("%08X %4X %s\n", ((assembl*)e)->decalage, ((assembl*)e)->code_ligne >> (4*(8- ((assembl*)e)->typ_aff)), ((assembl*)e)->ligne);
  }
  else if(((assembl*)e)->typ_aff == 5){
    printf("%08X %04X     %s\n", ((assembl*)e)->decalage, ((assembl*)e)->code_ligne, ((assembl*)e)->ligne);
  }
  else if(((assembl*)e)->typ_aff == 6){
    printf("%08X %6X %s\n", ((assembl*)e)->decalage, ((assembl*)e)->code_ligne >> (4*(8- ((assembl*)e)->typ_aff)), ((assembl*)e)->ligne);
  }
  else if(((assembl*)e)->typ_aff == 7){
    printf("%08X %08X %s\n", ((assembl*)e)->decalage, ((assembl*)e)->code_ligne, ((assembl*)e)->ligne);
  }
  else if(((assembl*)e)->typ_aff == 8){
    printf("%08X %8X %s\n", ((assembl*)e)->decalage, ((assembl*)e)->code_ligne >> (4*(8- ((assembl*)e)->typ_aff)), ((assembl*)e)->ligne);
  }
}

void ecrire_liste_assemblage(FILE* fichier, Liste p){
  while (!liste_vide(p)){
    fprintf(fichier,"%3i ", ((assembl*)p->val)->num_ligne);
    if(((assembl*)p->val)->typ_aff==9){
      fprintf(fichier,"                  %s\n",((assembl*)p->val)->ligne);
    }
    else if(((assembl*)p->val)->typ_aff == 0){
      fprintf(fichier,"%08X %08X %s\n", ((assembl*)p->val)->decalage, ((assembl*)p->val)->code_ligne, ((assembl*)p->val)->ligne);
    }
    else if(((assembl*)p->val)->typ_aff == 10){
      fprintf(fichier,"%08X 0000...  %s\n", ((assembl*)p->val)->decalage, ((assembl*)p->val)->ligne);
    }
    else if(((assembl*)p->val)->typ_aff == 1){
      fprintf(fichier,"%08X %s\n", ((assembl*)p->val)->decalage, ((assembl*)p->val)->ligne);
    }
    else if(((assembl*)p->val)->typ_aff == 2){
      fprintf(fichier,"%08X %2X %s\n", ((assembl*)p->val)->decalage, ((assembl*)p->val)->code_ligne >> (4*(8- ((assembl*)p->val)->typ_aff)), ((assembl*)p->val)->ligne);
    }
    else if(((assembl*)p->val)->typ_aff == 3){
      fprintf(fichier,"%08X %02X       %s\n", ((assembl*)p->val)->decalage, ((assembl*)p->val)->code_ligne, ((assembl*)p->val)->ligne);
    }
    else if(((assembl*)p->val)->typ_aff == 4){
      fprintf(fichier,"%08X %4X %s\n", ((assembl*)p->val)->decalage, ((assembl*)p->val)->code_ligne >> (4*(8- ((assembl*)p->val)->typ_aff)), ((assembl*)p->val)->ligne);
    }
    else if(((assembl*)p->val)->typ_aff == 5){
      fprintf(fichier,"%08X %04X     %s\n", ((assembl*)p->val)->decalage, ((assembl*)p->val)->code_ligne, ((assembl*)p->val)->ligne);
    }
    else if(((assembl*)p->val)->typ_aff == 6){
      fprintf(fichier,"%08X %6X %s\n", ((assembl*)p->val)->decalage, ((assembl*)p->val)->code_ligne >> (4*(8- ((assembl*)p->val)->typ_aff)), ((assembl*)p->val)->ligne);
    }
    else if(((assembl*)p->val)->typ_aff == 7){
      fprintf(fichier,"%08X %08X %s\n", ((assembl*)p->val)->decalage, ((assembl*)p->val)->code_ligne, ((assembl*)p->val)->ligne);
    }
    else if(((assembl*)p->val)->typ_aff == 8){
      fprintf(fichier,"%08X %8X %s\n", ((assembl*)p->val)->decalage, ((assembl*)p->val)->code_ligne >> (4*(8- ((assembl*)p->val)->typ_aff)), ((assembl*)p->val)->ligne);
    }
    p=p->suiv;
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
  a->ligne = calloc(1,sizeof(char)*strlen(line)+1);
  strcpy(a->ligne,line);
  a->num_ligne = line_num;
  a->typ_aff = 9;
  return a;
}

Liste chercher_ligne_col(int num_ligne, Liste col){
  if(liste_vide(col)) return NULL;
  Liste p_col = col;
  if(((instruction*)p_col->val)->inst->lex.nl==num_ligne){
    return p_col;
  }
  while(!liste_vide(p_col->suiv)) {
    p_col=p_col->suiv;
    if(((instruction*)p_col->val)->inst->lex.nl==num_ligne){
      return p_col;
    }
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
    p_col=p_col->suiv;
    if(((symb*)p_col->val)->lex.nl==num_ligne){
      return p_col;
    }
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
    p_ass=p_ass->suiv;
    if(((assembl*)p_ass->val)->num_ligne==num_ligne){
      return p_ass;
    }
  }
  return NULL;
}

void calcul_code_assemblage(Liste col_text, Liste col_data, Liste col_bss, Liste* assembl_l, unsigned int* n_lines, symb* symb_t) {
  int i, j, num_op, pseudo, compteur=0;
  Liste temp;
  Liste temp_ass;
  unsigned long code;
  symb* etiqu;

  for(i=0;i<*n_lines;i++) {

    temp = chercher_ligne_col(i,col_text);

    if(!liste_vide(temp)) {

      temp_ass = chercher_ligne_ass(i,assembl_l);
      if(!liste_vide(temp->suiv) && ((instruction*)temp->suiv->val)->inst->lex.nl == i) pseudo=1;
      else pseudo = 0;

      for(j=0;j<pseudo+1;j++) {
        if(j == 1) {
          assembl* new_ass = init_assembl("", i);
          temp_ass=inserer_element((void*)new_ass, temp_ass);
          temp=temp->suiv;
          *n_lines +=1;
        }

        for(num_op=0;num_op<((instruction*)temp->val)->inst_def.nb_op;num_op++) {

          switch(((instruction*)temp->val)->Operande[num_op].ope_typ){
            case ETI:
              etiqu = rech_mot_symb(((instruction*)temp->val)->Operande[num_op].ope_val->eti,symb_t);
              if(!strcmp(((instruction*)temp->val)->inst->lex.tok,"LW") || !strcmp(((instruction*)temp->val)->inst->lex.tok,"SW")) {
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
        ((assembl*)temp_ass->val)->typ_aff = 0;
      }
    }

    temp = chercher_ligne_col(i,col_data);
    if(!liste_vide(temp)){
      temp_ass = chercher_ligne_ass(i,assembl_l);

      if(!strcmp(((instruction*)temp->val)->inst->lex.tok,".word")) {
        for(num_op=0;num_op<((instruction*)temp->val)->inst_def.nb_op;num_op++) {
          if(num_op==0) {
            if(((instruction*)temp->val)->Operande[num_op].ope_typ == ETI) {
              etiqu = rech_mot_symb(((instruction*)temp->val)->Operande[num_op].ope_val->eti,symb_t);
              ((assembl*)temp_ass->val)->code_ligne = etiqu->deca;
            }
            else ((assembl*)temp_ass->val)->code_ligne = ((instruction*)temp->val)->Operande[num_op].ope_val->wrd;
            ((assembl*)temp_ass->val)->decalage = ((instruction*)temp->val)->inst->deca;
          }
          else {
            temp_ass = inserer_element((void*)init_assembl("",i),temp_ass);
            if(((instruction*)temp->val)->Operande[num_op].ope_typ == ETI) {
              etiqu = rech_mot_symb(((instruction*)temp->val)->Operande[num_op].ope_val->eti,symb_t);
              ((assembl*)temp_ass->val)->code_ligne = etiqu->deca;
            }
            else((assembl*)temp_ass->val)->code_ligne = ((instruction*)temp->val)->Operande[num_op].ope_val->wrd;
            ((assembl*)temp_ass->val)->decalage = ((instruction*)temp->val)->inst->deca+num_op*4;
          }
        ((assembl*)temp_ass->val)->typ_aff = 0;
        }
      }

      if(!strcmp(((instruction*)temp->val)->inst->lex.tok,".byte")) {
        compteur = 0;
        for(num_op=0;num_op<((instruction*)temp->val)->inst_def.nb_op;num_op++) {
          if(num_op%4) {
            ((assembl*)temp_ass->val)->code_ligne |= ((instruction*)temp->val)->Operande[num_op].ope_val->wrd << 4*(4-2*compteur);
            compteur += 1;
          }
          else {
            if(num_op!=0) {
              temp_ass = inserer_element((void*)init_assembl("",i),temp_ass);
              ((assembl*)temp_ass->val)->code_ligne = ((instruction*)temp->val)->Operande[num_op].ope_val->wrd << 24;
              compteur = 0;
              ((assembl*)temp_ass->val)->decalage = ((instruction*)temp->val)->inst->deca+4;
            }
            else {
            ((assembl*)temp_ass->val)->code_ligne = ((instruction*)temp->val)->Operande[num_op].ope_val->wrd << 24;
            compteur = 0;
            ((assembl*)temp_ass->val)->decalage = ((instruction*)temp->val)->inst->deca;
            }
          }
          ((assembl*)temp_ass->val)->typ_aff = 2* (compteur+1);
        }
      }

/*      if(strcmp(((instruction*)temp->val)->inst->lex.tok,".asciiz")) {
        compteur = 0;
        for(num_op=0;num_op<((instruction*)temp->val)->inst_def.nb_op;num_op++) {
          for(j=0;j<strlen(((instruction*)temp->val)->Operande[num_op].ope_val->chaine);j++) {
            if(j%4) {
              ((assembl*)temp_ass->val)->code_ligne |= ((instruction*)temp->val)->Operande[num_op].ope_val->chaine[j] << 4*(4-2*compteur);
              printf("%08x\n", ((assembl*)temp_ass->val)->code_ligne);
              compteur += 1;
            }
            else {
              if(j!=0) {
                temp_ass = inserer_element((void*)init_assembl("",i),temp_ass);
                ((assembl*)temp_ass->val)->code_ligne = ((instruction*)temp->val)->Operande[num_op].ope_val->chaine[j] << 24;
                compteur = 0;
                ((assembl*)temp_ass->val)->decalage = ((instruction*)temp->val)->inst->deca+4;
              }
              else {
              ((assembl*)temp_ass->val)->code_ligne = ((instruction*)temp->val)->Operande[num_op].ope_val->wrd << 24;
              compteur = 0;
              ((assembl*)temp_ass->val)->decalage = ((instruction*)temp->val)->inst->deca;
              }
            }
            ((assembl*)temp_ass->val)->typ_aff = 2* (compteur+1);
            printf("%0d\n", ((assembl*)temp_ass->val)->typ_aff);
          }
        }
      }*/

      if(!strcmp(((instruction*)temp->val)->inst->lex.tok,".space")) {
        ((assembl*)temp_ass->val)->code_ligne = 0;
        if(((instruction*)temp->val)->Operande[0].ope_val->wrd >= 3) {
          ((assembl*)temp_ass->val)->typ_aff=10;
        }
        else ((assembl*)temp_ass->val)->typ_aff=2*(((instruction*)temp->val)->Operande[0].ope_val->wrd+1) - 1;
        ((assembl*)temp_ass->val)->decalage = ((instruction*)temp->val)->inst->deca;
      }
    }

    temp = chercher_ligne_bss(i,col_bss);
    if(!liste_vide(temp)){
      temp_ass = chercher_ligne_ass(i,assembl_l);
      ((assembl*)temp_ass->val)->code_ligne = 0;
      if(atoi(((symb*)temp->suiv->val)->lex.tok) >= 3) {
        ((assembl*)temp_ass->val)->typ_aff=10;
      }
      else ((assembl*)temp_ass->val)->typ_aff=2*(atoi(((symb*)temp->suiv->val)->lex.tok)+1) - 1;
      ((assembl*)temp_ass->val)->decalage = ((symb*)temp->val)->deca;
    }
  }
  return ;
}
