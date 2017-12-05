#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include<lex.h>
#include<liste.h>
#include<symb.h>
#include<dico.h>
#include<verif.h>

int recherche_instr(char* instr,inst_def_t* dico,int taille){
	inst_def_t* p=dico;
	while (p-dico<taille){
		if (strcmp(instr,p->symbole)==0){
			return (p-dico);}
		p++;}
	return (-1);}

symb* rech_mot(char* mot, symb* tab){
	int i=hachage(mot);
	if((tab+i)->lex.tok!=0){
		if(strcmp((tab+i)->lex.tok,mot)==0) {return tab+i;}}
	return NULL;}

Liste verif_arg_data(Liste* data_l){
	Liste list_instr=creer_liste();
	Liste p=*data_l;
	if (liste_vide(p)){
		printf("Pas d'instruction dans text ! \n");
		return list_instr;}
	symb* temp=p->val;
	symb* current_inst=temp;
	instruction* instr;
	while(!liste_vide(p)){
		switch (temp->lex.typ){
			case DIRECTIVE:
				current_inst=temp;
				p=p->suiv;if (!liste_vide(p)){temp=p->val;}
				while (temp->lex.typ!=DIRECTIVE && !liste_vide(p)){
					instr=calloc(1,sizeof(instruction));
					instr->Operande=calloc(1,sizeof(opestruct));
					instr->inst=current_inst;
					switch(temp->lex.typ){
						case SYMBOLE:
							instr->Operande->ope_val=calloc(1,sizeof(char*));
							(instr->Operande)->ope_typ=ETI;
							(instr->Operande)->ope_val->eti=temp->lex.tok;
							break;
						case DECIMAL:
							instr->Operande->ope_val=calloc(1,sizeof(unsigned long));
							(instr->Operande)->ope_typ=WRD;
							(instr->Operande)->ope_val->wrd=(unsigned long)atoi(temp->lex.tok);
							break;
						case CHAINE:
							instr->Operande->ope_val=calloc(1,sizeof(char*));
							(instr->Operande)->ope_typ=CHN;
							(instr->Operande)->ope_val->chaine=temp->lex.tok;	
						default:
							break;}
					p=p->suiv;if(!liste_vide(p)){temp=p->val;}
					list_instr=ajout_queue(instr,list_instr);}
				break;
			default:
				p=p->suiv;if (!liste_vide(p)){temp=p->val;}
				break;}}
	return list_instr;}
		


Liste verif_arg_text(Liste* text_l,inst_def_t* dico, int taille){
	Liste list_instr=creer_liste();
	instruction* instr;
	Liste p=*text_l,q=p;
	if (liste_vide(p)){
		printf("Pas d'instruction dans text ! \n");
		return list_instr;}
	symb* temp=p->val;
	symb* current_inst=temp;
	int index_dico,nb_op,current_nl,i;
	char type;
	char* arg;
	while (!liste_vide(p)){
		current_nl=temp->lex.nl;
		switch (temp->lex.typ){
			case SYMBOLE:
				strtoupper(temp->lex.tok);
				current_inst=temp;
				index_dico=recherche_instr(temp->lex.tok,dico,taille);
				if(!liste_vide(p->suiv)){
					p=p->suiv;temp=p->val;}
				if (index_dico>=0){
					q=p;
					nb_op=dico[index_dico].nb_op;
					type=dico[index_dico].type;
					arg=dico[index_dico].arg;
					if (type=='R'){
						while (current_nl-temp->lex.nl==0 && !liste_vide(p)){
							switch (temp->lex.typ){
								case REGISTRE:
									if (nb_op>1){;}
									else if (nb_op==1 && strcmp(arg,"R")==0){;}
									else{
										printf("Argument %s invalide dans le %s ligne %d \n",temp->lex.tok,dico[index_dico].symbole,current_nl);
									current_inst->lex.typ=ERREUR;}
									nb_op=nb_op-1;
									break;
								case VIRGULE:
									break;
								case DECIMAL:
									if (nb_op==1 && strcmp(arg,"SA")==0){;}
									else if (nb_op>1 && strcmp(arg,"SA")==0){
										printf("Shift ammount pas a la bonne place dans le %s ligne %d \n",dico[index_dico].symbole,current_nl);
										current_inst->lex.typ=ERREUR;}
									else{
										printf("Argument %s invalide dans le %s ligne %d \n",temp->lex.tok,dico[index_dico].symbole,current_nl);
										current_inst->lex.typ=ERREUR;}
									nb_op=nb_op-1;
									break;
								default:
									temp->lex.typ=ERREUR;
									nb_op=nb_op-1;
									break;}
							p=p->suiv;if(!liste_vide(p)){temp=p->val;}}
						if(current_inst->lex.typ!=ERREUR){
							instr=calloc(1,sizeof(instruction));
							instr->inst=current_inst;
							instr->inst_def=dico[index_dico];
							i=0;
							instr->Operande=calloc(dico[index_dico].nb_op,sizeof(opestruct));
							while (i<dico[index_dico].nb_op){
								temp=q->val;
								switch(temp->lex.typ){
									case DECIMAL:
										instr->Operande[i].ope_val=calloc(dico[index_dico].nb_op,sizeof(unsigned short));
										(instr->Operande)[i].ope_typ=SA;
										(instr->Operande)[i].ope_val->sa=(unsigned short)atoi(temp->lex.tok);
										break;
									case REGISTRE:
										instr->Operande[i].ope_val=calloc(dico[index_dico].nb_op,sizeof(char*));
										(instr->Operande)[i].ope_typ=REG;
										(instr->Operande)[i].ope_val->reg=temp->lex.tok;
										break;
									default:
										break;}
								i++;
								q=q->suiv;if(!liste_vide(q)){temp=q->val;}}
							list_instr=ajout_queue(instr,list_instr);}}
					else if (type=='I'){
						while (current_nl-temp->lex.nl==0 && !liste_vide(p)){
							switch (temp->lex.typ){
								case REGISTRE:
									if (nb_op>1){;}
									else{
										printf("Argument %s invalide dans le %s ligne %d \n",temp->lex.tok,dico[index_dico].symbole,current_nl);}
									nb_op=nb_op-1;
									break;
								case VIRGULE:
									break;
								case DECIMAL:
									if (nb_op==1 && strcmp(arg,"I")==0){;}
									else if (nb_op>1){
										printf("Immediate pas a la bonne place dans le %s ligne %d \n",dico[index_dico].symbole,current_nl);}
									else{
										printf("Argument %s invalide dans le %s ligne %d \n",temp->lex.tok,dico[index_dico].symbole,current_nl);}
									nb_op=nb_op-1;
									break;
								case SYMBOLE:
									if (nb_op==1 && strcmp(arg,"I")==0){;}
									else if (nb_op>1){
										printf("Immediate pas a la bonne place dans le %s ligne %d \n",dico[index_dico].symbole,current_nl);printf("%s \n",temp->lex.tok);}
									else{
										printf("Argument %s invalide dans le %s ligne %d \n",temp->lex.tok,dico[index_dico].symbole,current_nl);}
									nb_op=nb_op-1;
									break;
								default:
									temp->lex.typ=ERREUR;
									nb_op=nb_op-1;
									break;}
							p=p->suiv;if(!liste_vide(p)){temp=p->val;}}
						if(current_inst->lex.typ!=ERREUR){
							instr=calloc(1,sizeof(instruction));
							instr->inst=current_inst;
							instr->inst_def=dico[index_dico];
							i=0;
							instr->Operande=calloc(dico[index_dico].nb_op,sizeof(opestruct));
							while (i<dico[index_dico].nb_op){
								temp=q->val;
								switch(temp->lex.typ){
									case SYMBOLE:
										instr->Operande[i].ope_val=calloc(dico[index_dico].nb_op,sizeof(char*));
										(instr->Operande)[i].ope_typ=ETI;
										(instr->Operande)[i].ope_val->eti=temp->lex.tok;
										break;
									case REGISTRE:
										instr->Operande[i].ope_val=calloc(dico[index_dico].nb_op,sizeof(char*));
										(instr->Operande)[i].ope_typ=REG;
										(instr->Operande)[i].ope_val->reg=temp->lex.tok;
										break;
									case DECIMAL:
										instr->Operande[i].ope_val=calloc(dico[index_dico].nb_op,sizeof(short));
										(instr->Operande)[i].ope_typ=IMD;
										(instr->Operande)[i].ope_val->imd=(short)atoi(temp->lex.tok);
										break;
									default:
										break;}
								i++;
								q=q->suiv;if(!liste_vide(q)){temp=q->val;}}
							list_instr=ajout_queue(instr,list_instr);}}
					else if (type=='J'){
						while (current_nl-temp->lex.nl==0 && !liste_vide(p)){
							switch (temp->lex.typ){
								case REGISTRE:
									if (nb_op>1){;}
									else{
										printf("Argument %s invalide dans le %s ligne %d \n",temp->lex.tok,dico[index_dico].symbole,current_nl);}
									nb_op=nb_op-1;
									break;
								case SYMBOLE:
									if (nb_op==1){;}
									else{
										printf("Etiquette pas a la bonne place dans le %s ligne %d \n",dico[index_dico].symbole,current_nl);}
									nb_op=nb_op-1;
									break;
								case VIRGULE:
									break;
								case DECIMAL:
									nb_op=nb_op-1;
									break;
								default:
									temp->lex.typ=ERREUR;
									printf("Argument %s invalide dans le %s ligne %d \n",temp->lex.tok,dico[index_dico].symbole,current_nl);
									nb_op=nb_op-1;
									break;}
							p=p->suiv;if(!liste_vide(p)){temp=p->val;}}
						if(current_inst->lex.typ!=ERREUR){
							instr=calloc(1,sizeof(instruction));
							instr->inst=current_inst;
							instr->inst_def=dico[index_dico];
							i=0;
							instr->Operande=calloc(dico[index_dico].nb_op,sizeof(opestruct));
							while (i<dico[index_dico].nb_op){
								temp=q->val;
								switch(temp->lex.typ){
									case SYMBOLE:
										instr->Operande[i].ope_val=calloc(dico[index_dico].nb_op,sizeof(char*));
										(instr->Operande)[i].ope_typ=ETI;
										(instr->Operande)[i].ope_val->eti=temp->lex.tok;
										break;
									case REGISTRE:
										instr->Operande[i].ope_val=calloc(dico[index_dico].nb_op,sizeof(char*));
										(instr->Operande)[i].ope_typ=REG;
										(instr->Operande)[i].ope_val->reg=temp->lex.tok;
										break;
									case DECIMAL:
										instr->Operande[i].ope_val=calloc(dico[index_dico].nb_op,sizeof(short));
										(instr->Operande)[i].ope_typ=IMD;
										(instr->Operande)[i].ope_val->imd=(short)atoi(temp->lex.tok);
										break;
									default:
										break;}
								i++;
								q=q->suiv;if(!liste_vide(q)){temp=q->val;}}
							list_instr=ajout_queue(instr,list_instr);}}}
				else{
					current_inst->lex.typ=ERREUR;
					while (current_nl-temp->lex.nl==0 && !liste_vide(p)){
						p=p->suiv;if(!liste_vide(p)){temp=p->val;}}}
				break;
			default:
				temp->lex.typ=ERREUR;
				p=p->suiv;if(!liste_vide(p)){temp=p->val;}
				break;}}
	return list_instr;}
