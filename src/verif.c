#include<verif.h>

char* reg_mnemo[] = {"$zero", "$0", "$at", "$1", "$v0", "$2", "$v1", "$3", "$a0", "$4", "$a1", "$5", "$a2", "$6", "$a3", "$7",
 "$t0", "$8", "$t1", "$9", "$t2", "$10", "$t3", "$11", "$t4", "$12", "$t5", "$13", "$t6", "$14", "$t7", "$15", "$s0",
  "$16", "$s1", "$17", "$s2", "$18", "$s3", "$19", "$s4", "$20", "$s5", "$21", "$s6", "$22", "$s7", "$23", "$t8",
  "$24", "$t9", "$25", "$k0", "$26", "$k1", "$27", "$gp", "$28", "$sp", "$29", "$fp", "$30", "$ra", "$31"};

int recherche_instr(char* instr,inst_def_t* dico,int taille){
	inst_def_t* p=dico;
	while (p-dico<taille){
		if (strcmp(instr,p->symbole)==0){
			return (p-dico);}
		p++;}
	return (-1);}

void affiche_liste_ope_text(Liste arg_text){
    printf("\n-------------------------Collection Operande text-----------------------------\n");
    int i;
    Liste p=arg_text;
    instruction* inst;
    while(!liste_vide(p)){
	i=0;
	inst=p->val;
	while (i<inst->inst_def.nb_op && !liste_vide(p)){
		if(inst->Operande[i].ope_typ==IMD){
			printf("Opérande %hd instruction %s ligne %d\n",inst->Operande[i].ope_val->imd,inst->inst->lex.tok,inst->inst->lex.nl);}
		else if(inst->Operande[i].ope_typ==REG){
			printf("Opérande %s instruction %s ligne %d\n",inst->Operande[i].ope_val->reg,inst->inst->lex.tok,inst->inst->lex.nl);}
		else if(inst->Operande[i].ope_typ==ETI){
			printf("Opérande %s instruction %s ligne %d\n",inst->Operande[i].ope_val->eti,inst->inst->lex.tok,inst->inst->lex.nl);}
		else if(inst->Operande[i].ope_typ==SA){
			printf("Opérande %hd instruction %s ligne %d\n",inst->Operande[i].ope_val->sa,inst->inst->lex.tok,inst->inst->lex.nl);}
		i++;}
	p=p->suiv;}}

void affiche_liste_ope_data(Liste arg_data){
    printf("\n-------------------------Collection Operande data-----------------------------\n");
    Liste p=arg_data;
    instruction* inst;
    while(!liste_vide(p)){
	inst=p->val;
	if(inst->Operande->ope_typ==WRD){
		printf("Opérande %lu instruction %s ligne %d\n",inst->Operande->ope_val->wrd,inst->inst->lex.tok,inst->inst->lex.nl);}
	else if(inst->Operande->ope_typ==CHN){
		printf("Opérande %s instruction %s ligne %d\n",inst->Operande->ope_val->chaine,inst->inst->lex.tok,inst->inst->lex.nl);}
	else if(inst->Operande->ope_typ==ETI){
		printf("Opérande %s instruction %s ligne %d\n",inst->Operande->ope_val->eti,inst->inst->lex.tok,inst->inst->lex.nl);}
	p=p->suiv;}}

char* rech_mot(char* mot, char** tab){
	int i=0;
	while (i<64){
		if(strcmp(tab[i],mot)==0){
			if(i%2==0){return tab[i+1];}
			else if (i%2==1){return tab[i];}}
		i++;}
	return NULL;}

symb* rech_mot_symb(char* mot, symb* tab){
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
              instr->inst_def.arg=strdup("WRD");
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



Liste verif_arg_text(Liste* text_l,inst_def_t* dico, int taille, symb* tab){
	Liste list_instr=creer_liste();
	instruction* instr;
	Liste p=*text_l,q=p;
	if (liste_vide(p)){
		printf("Pas d'instruction dans text ! \n");
		return list_instr;}
	symb* temp=(symb*)p->val;
	symb* current_inst=temp;
	int index_dico,nb_op,current_nl,i;
	char type;
	char* arg;
	char* mnemo;
	while (!liste_vide(p)){
		current_nl=temp->lex.nl;
		switch (temp->lex.typ){
			case SYMBOLE:
				strtoupper((temp->lex).tok);
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
									mnemo=rech_mot(temp->lex.tok,reg_mnemo);
									if(mnemo!=NULL){
										temp->lex.tok=mnemo;}
									else{
										printf("Nom de registre inconnu pour %s \n",temp->lex.tok);
										current_inst->lex.typ=ERREUR;
										temp->lex.typ=ERREUR;}
									if (nb_op>1){;}
									else if (nb_op==1 && strcmp(arg,"R")==0){;}
									else{
										printf("Argument %s invalide dans le %s ligne %d \n",temp->lex.tok,dico[index_dico].symbole,current_nl);
										current_inst->lex.typ=ERREUR;
										temp->lex.typ=ERREUR;}
									nb_op=nb_op-1;
									break;
								case VIRGULE:
									break;
								case DECIMAL:
									if (nb_op==1 && strcmp(arg,"SA")==0){;}
									else if (nb_op>1 && strcmp(arg,"SA")==0){
										printf("Shift ammount pas a la bonne place dans le %s ligne %d \n",dico[index_dico].symbole,current_nl);
										current_inst->lex.typ=ERREUR;
										temp->lex.typ=ERREUR;}
									else{
										printf("Argument %s invalide dans le %s ligne %d \n",temp->lex.tok,dico[index_dico].symbole,current_nl);
										current_inst->lex.typ=ERREUR;
										temp->lex.typ=ERREUR;}
									nb_op=nb_op-1;
									break;
								default:
									temp->lex.typ=ERREUR;
									current_inst->lex.typ=ERREUR;
									nb_op=nb_op-1;
									break;}
							p=p->suiv;if(!liste_vide(p)){temp=p->val;}}
						if(nb_op==0 && current_inst->lex.typ!=ERREUR){
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
							list_instr=ajout_queue(instr,list_instr);}
						else{
							current_inst->lex.typ=ERREUR;}}
					else if (type=='I'){
						while (current_nl-temp->lex.nl==0 && !liste_vide(p)){
							switch (temp->lex.typ){
								case REGISTRE:
									mnemo=rech_mot(temp->lex.tok,reg_mnemo);
									if(mnemo!=NULL){
										temp->lex.tok=mnemo;}
									else{
										printf("Nom de registre inconnu pour %s \n",temp->lex.tok);
										temp->lex.typ=ERREUR;
										current_inst->lex.typ=ERREUR;}
									if (nb_op>1){;}
									else{
										printf("Argument %s invalide dans le %s ligne %d \n",temp->lex.tok,dico[index_dico].symbole,current_nl);
										current_inst->lex.typ=ERREUR;
										temp->lex.typ=ERREUR;}
									nb_op=nb_op-1;
									break;
								case VIRGULE:
									break;
								case DECIMAL:
									if (nb_op==1 && (strcmp(arg,"I")==0 || strcmp(arg,"REL")==0)){;}
									else if (nb_op>1){
										printf("Immediate pas a la bonne place dans le %s ligne %d \n",dico[index_dico].symbole,current_nl);
										current_inst->lex.typ=ERREUR;
										temp->lex.typ=ERREUR;}
									else{
										printf("Argument %s invalide dans le %s ligne %d \n",temp->lex.tok,dico[index_dico].symbole,current_nl);
										current_inst->lex.typ=ERREUR;
										temp->lex.typ=ERREUR;}
									nb_op=nb_op-1;
									break;
								case OFFSETBASE:
									if (nb_op==2 && strcmp(arg,"OFB")==0){
										Liste q=p->suiv,r=calloc(1,sizeof(Liste));
										p->suiv=r;
										r->suiv=q;
										int i=0,j=0;
										while (temp->lex.tok[i]!='('){
											i++;}
										j=i;
										while (temp->lex.tok[j]!=')'){
											j++;}
										temp->lex.tok[j]='\0';
										mnemo=rech_mot(temp->lex.tok+i+1,reg_mnemo);
										if(mnemo!=NULL){
											symb* tempo=calloc(1,sizeof(symb));
											tempo->lex.tok=mnemo;
											tempo->lex.typ=REGISTRE;
											p->suiv->val=tempo;
											nb_op=nb_op-1;}
										else{
											printf("Nom de registre inconnu pour %s \n",((symb*)p->suiv->val)->lex.tok);
											temp->lex.typ=ERREUR;
											current_inst->lex.typ=ERREUR;}
										temp->lex.tok[i]='\0';
										to_decimal(temp->lex.tok);
										temp->lex.typ=DECIMAL;
										p=p->suiv;if(!liste_vide(p)){temp=p->val;}}
									else if (nb_op>1){
										printf("Offset base pas a la bonne place dans le %s ligne %d \n",dico[index_dico].symbole,current_nl);
										current_inst->lex.typ=ERREUR;
										temp->lex.typ=ERREUR;}
									else{
										printf("Argument %s invalide dans le %s ligne %d \n",temp->lex.tok,dico[index_dico].symbole,current_nl);
										current_inst->lex.typ=ERREUR;
										temp->lex.typ=ERREUR;}
									nb_op=nb_op-1;
									break;
								case SYMBOLE:
									if (rech_mot_symb(temp->lex.tok,tab)==NULL){
										temp->section=undefined;
										temp->deca=0;
										ajout_tab(tab,temp);}
									if (nb_op==1 && (strcmp(arg,"I")==0 || strcmp(arg,"REL")==0)){;}
									else if (nb_op>1){
										printf("Immediate pas a la bonne place dans le %s ligne %d \n",dico[index_dico].symbole,current_nl);printf("%s \n",temp->lex.tok);
										current_inst->lex.typ=ERREUR;
										temp->lex.typ=ERREUR;}
									else{
										printf("Argument %s invalide dans le %s ligne %d \n",temp->lex.tok,dico[index_dico].symbole,current_nl);
										current_inst->lex.typ=ERREUR;
										temp->lex.typ=ERREUR;}
									nb_op=nb_op-1;
									break;
								default:
									current_inst->lex.typ=ERREUR;
									temp->lex.typ=ERREUR;
									nb_op=nb_op-1;
									break;}
							p=p->suiv;if(!liste_vide(p)){temp=p->val;}}
						if(nb_op==0 && current_inst->lex.typ!=ERREUR){
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
							list_instr=ajout_queue(instr,list_instr);}
						else{
							current_inst->lex.typ=ERREUR;}}
					else if (type=='J'){
						while (current_nl-temp->lex.nl==0 && !liste_vide(p)){
							switch (temp->lex.typ){
								case REGISTRE:
									mnemo=rech_mot(temp->lex.tok,reg_mnemo);
									if(mnemo!=NULL){
										temp->lex.tok=mnemo;}
									else{
										printf("Nom de registre inconnu pour %s \n",temp->lex.tok);
										current_inst->lex.typ=ERREUR;
										temp->lex.typ=ERREUR;}
									if (nb_op>1){;}
									else{
										printf("Argument %s invalide dans le %s ligne %d \n",temp->lex.tok,dico[index_dico].symbole,current_nl);
										current_inst->lex.typ=ERREUR;
										temp->lex.typ=ERREUR;}
									nb_op=nb_op-1;
									break;
								case SYMBOLE:
									if (rech_mot_symb(temp->lex.tok,tab)==NULL){
										temp->section=undefined;
										temp->deca=0;
										ajout_tab(tab,temp);}
									if (nb_op==1){;}
									else{
										printf("Etiquette pas a la bonne place dans le %s ligne %d \n",dico[index_dico].symbole,current_nl);
										current_inst->lex.typ=ERREUR;
										temp->lex.typ=ERREUR;}
									nb_op=nb_op-1;
									break;
								case VIRGULE:
									break;
								case DECIMAL:
									nb_op=nb_op-1;
									break;
								default:
									current_inst->lex.typ=ERREUR;
									temp->lex.typ=ERREUR;
									nb_op=nb_op-1;
									break;}
							p=p->suiv;if(!liste_vide(p)){temp=p->val;}}
						if(nb_op==0 && current_inst->lex.typ!=ERREUR){
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
							list_instr=ajout_queue(instr,list_instr);}
						else{
							current_inst->lex.typ=ERREUR;}}}
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
