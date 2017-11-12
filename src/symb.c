#define N 100

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include<liste.h>
#include<lex.h>
#include<symb.h>

void affiche_tab(symb* s){ /* Affiche uniquement les etiquettes stockées */
	symb* t=s;
	char* secti[]= {"bss", "data", "text", "INI"};
	while(t!=s+N){
	if(t->lex.tok!=0){
		printf("On a stocké l'étiquette %s de decalage %d dans la section %s \n",t->lex.tok,t->deca,secti[t->section]);}
		t++;}}

void affiche_liste(Liste* l){ /* Affiche les listes d'instruction constitutées */
	char* secti[]= {"bss", "data", "text", "INI"};
	Liste u=*l;        
	while(!liste_vide(u)){
		if(((symb*)u->val)->lex.typ==DIRECTIVE || ((symb*)u->val)->lex.typ==SYMBOLE){
			printf("On a stocké l'instruction %s avec un decalage de %d dans %s \n",((symb*)u->val)->lex.tok,((symb*)u->val)->deca,secti[((symb*)u->val)->section]);}
		else{
			printf("On a stocké l'operande %s avec un decalage de %d dans %s \n",((symb*)u->val)->lex.tok,((symb*)u->val)->deca,secti[((symb*)u->val)->section]);}
	u=u->suiv;}}

void ajout_liste(Liste* l, Liste p,int t,int* d){ /*Fonction qui allonge et rajoute un élément a la liste */
	symb* temp=calloc(1,sizeof(symb));
	temp->lex=*((lexeme*)p->val);
	temp->deca=d[t];
	temp->section=t;
	*l=ajout_queue(temp,*l);}

Liste* init_liste(){ /* Fonction qui alloue l'espace our une liste */
	Liste* l=calloc(1,sizeof(*l));
	*l=creer_liste();
	return l;}

int hachage(char* mot){ /* Fonction de hachage */
	int i=0;
	int h=0;
	int a=1;
	while (mot[i]!='\0') {
		h+=((int)mot[i]-'a')*a;
		a*=31;
		i++;}
	if (h%N>=0){return h%N;}
	else{return (h%N+100);}}

void ajout_tab(symb* s, symb* t){ /* Fonction qui ajoute a la table de hachage */
	int i=hachage(t->lex.tok);
	s[i]=*t;
	return;}

int deca_word (int d){ /* Fonction decalage pour .word */
	if (d%4==0){return (d+4);}
	else {d=d-d%4;
	return (d+8);}}

void init_symb(symb* t){ /* Fonction qui renvoie a l'etat initial */
	t->section=INI;}

void tabl_symb(Liste l, symb* s, Liste* data_l, Liste* text_l, Liste* bss_l){
	int* d=calloc(3,sizeof(int));/* Stockage et initialisation des decalages [bss,data,text] */
	Liste p=l->suiv;
	lexeme* temp=p->val;
	symb* t=calloc(1,sizeof(symb));
	init_symb(t);
	int current_l; /* Valeur stockée de la ligne actuellement examinée */
	while (!liste_vide(p)){
		current_l=temp->nl;
		switch (t->section){
		case INI:
			switch (temp->typ){
				case DIRECTIVE:
					if (strcmp(temp->tok,".bss")==0){t->section=bss;}
					else if (strcmp(temp->tok,".data")==0){t->section=data;}
					else if (strcmp(temp->tok,".text")==0){t->section=text;}
					else{temp->typ=ERREUR;}/* ERREUR, on peut pas mettre une autre directive de ces trois là */
					break;
				case COMMENT:
					break;		
				default:
					temp->typ=ERREUR;/* ERREUR, on veut forcément un COMMENT ou une DIRECTIVE */
					break;}
			break;
		case bss:
			switch (temp->typ){
				case DIRECTIVE:
					if (strcmp((temp->tok),".bss")==0){;} /* On change pas de section */
					else if (strcmp(temp->tok,".data")==0){init_symb(t);}
					else if (strcmp(temp->tok,".text")==0){init_symb(t);}
					else if (strcmp(temp->tok,".space")==0 && !liste_vide(p->suiv)){
						ajout_liste(bss_l,p,t->section,d);
						p=p->suiv;
						temp=p->val;
						while ((current_l-temp->nl && !liste_vide(p->suiv))==0){
							switch(temp->typ){
								case DECIMAL: /* Test validité arguments */
									ajout_liste(bss_l,p,t->section,d);
									d[bss]=d[bss]+atoi(temp->tok);
									break;
								case VIRGULE:
									break;
								default:
									temp->typ=ERREUR;/* ERREUR, on attend que VIRGULE et DECIMAL en argument */
									break;}
							p=p->suiv;
							temp=p->val;}}								
					else{temp->typ=ERREUR;}/* ERREUR, pas d'autres directives acceptées dans .bss */
					break;
				case COMMENT:
					break;
				case SYMBOLE:
					if (!liste_vide(p->suiv) && !liste_vide(p->suiv->suiv)){
						if (((lexeme*)p->suiv->val)->typ==DEUX_PTS){
							ajout_liste(bss_l,p,t->section,d);
							t->lex=*temp;
							t->deca=d[bss];
							ajout_tab(s,t);
							p=p->suiv;
							temp=p->val;}
						else{
							temp->typ=ERREUR;}}/* ERREUR, on veut pas d'instruction */
					break;
				default:
					temp->typ=ERREUR;/* ERREUR, on attend que DIRECTIVE ou COMMENT ou ETIQUETTE dans cette section */
					break;}
			break;
		case data:
			switch (temp->typ){
				case DIRECTIVE:
					if (strcmp(temp->tok,".data")==0){;}
					else if (strcmp(temp->tok,".bss")==0){init_symb(t);}
					else if (strcmp(temp->tok,".text")==0){init_symb(t);}
					else if (strcmp(temp->tok,".word")==0  && !liste_vide(p->suiv)){
						ajout_liste(data_l,p,t->section,d);
						p=p->suiv;
						temp=p->val;
						while (current_l-temp->nl==0  && !liste_vide(p->suiv)){
							switch (temp->typ){
								case HEXA:
									ajout_liste(data_l,p,t->section,d);
									d[data]=deca_word(d[data]);
								break;
								case DECIMAL:
									ajout_liste(data_l,p,t->section,d);
									d[data]=deca_word(d[data]);
									break;
								case VIRGULE:
									break;
								default:
									temp->typ=ERREUR;/* ERREUR, on attend un DECIMAL ou un HEXA */
									break;}
							p=p->suiv;
							temp=p->val;}}
					else if (strcmp((temp->tok),".byte")==0  && !liste_vide(p->suiv)){
						ajout_liste(data_l,p,t->section,d);
						p=p->suiv;
						temp=p->val;
						while (current_l-temp->nl==0  && !liste_vide(p->suiv)){
							switch(temp->typ){
								case DECIMAL:
									ajout_liste(data_l,p,t->section,d);
									d[data]=d[data]+1;
									break;
								case HEXA:
									ajout_liste(data_l,p,t->section,d);
									d[data]=d[data]+1;
									break;
								case VIRGULE:
									break;
								default:
									temp->typ=ERREUR;/* ERREUR, on attend que VIRGULE et DECIMAL en argument */
									break;}
							p=p->suiv;
							temp=p->val;}}
					else if (strcmp(temp->tok,".asciiz")==0 && !liste_vide(p->suiv)){
						ajout_liste(data_l,p,t->section,d);
						p=p->suiv;
						temp=p->val;
						while (current_l-temp->nl==0 && !liste_vide(p->suiv)){
							switch(temp->typ){
								case CHAINE:
									ajout_liste(data_l,p,t->section,d);
									d[data]=d[data]+strlen(temp->tok)-3;/*-3 Du aux espaces rajoutés par standardise + les guillemets*/
									break;
								case VIRGULE:
									break;
								default:
									temp->typ=ERREUR;/*ERREUR, on attend que VIRGULE et DECIMAL en argument */
									break;}
								p=p->suiv;
								temp=p->val;}}
					else if (strcmp((temp->tok),".space")==0){
						ajout_liste(data_l,p,t->section,d);
						while (current_l-temp->nl==0  && !liste_vide(p->suiv)){
							switch(temp->typ){
								case DECIMAL:
									ajout_liste(data_l,p,t->section,d);
									d[data]=d[data]+atoi(temp->tok);
									break;									
								case VIRGULE:
									break;
								default:
									temp->typ=ERREUR;/*ERREUR, on attend que VIRGULE et DECIMAL en argument */
									break;}
							p=p->suiv;
							temp=p->val;}}
					else{
						temp->typ=ERREUR;}/*ERREUR, directive dans mauvaise section */
					break;
				case COMMENT:
					break;
				case SYMBOLE:
					if (!liste_vide(p->suiv) && !liste_vide(p->suiv->suiv)){
						if (((lexeme*)p->suiv->val)->typ==DEUX_PTS){
							ajout_liste(bss_l,p,t->section,d);
							t->lex=*temp;
							t->deca=d[data];
							ajout_tab(s,t);
							p=p->suiv;
							temp=p->val;}
						else{
							temp->typ=ERREUR;/*ERREUR, on veut pas d'instruction */}}
					break;
					
				default:
					temp->typ=ERREUR;/*ERREUR, on attend que DIRECTIVE ou COMMENT ou ETIQUETTE */
					break;}
			break;
		case text:
			switch(temp->typ){
				case DIRECTIVE:
					if (strcmp(temp->tok,".text")==0){;}
					else if (strcmp(temp->tok,".bss")==0){init_symb(t);}
					else if (strcmp(temp->tok,".data")==0){init_symb(t);}
					else {
						temp->typ=ERREUR;/*ERREUR on attend aucune DIRECTIVE autre que de section */}
					break;
				case COMMENT:
					break;
				case SYMBOLE:
					if (!liste_vide(p->suiv) && !liste_vide(p->suiv->suiv)){
						if (((lexeme*)p->suiv->val)->typ==DEUX_PTS){
							ajout_liste(text_l,p,t->section,d);
							t->lex=*temp;
							t->deca=d[text];
							ajout_tab(s,t);
							p=p->suiv;
							temp=p->val;}
						else{
							while (current_l-temp->nl==0 && !liste_vide(p->suiv)){
								switch (temp->typ){
									case VIRGULE:
										break;
									default:
										ajout_liste(text_l,p,t->section,d);
										break;}
								p=p->suiv;
								temp=p->val;}
							d[text]=d[text]+4;}}
					break;
				default:
					temp->typ=ERREUR;/*ERREUR, on attend pas autre chose */
					break;}
			break;}
		/*printf ("Décalage de bss : %d, data : %d, text : %d a la ligne %d \n",d[0],d[1],d[2],((lexeme*)p->val)->nl);*/
		if (liste_vide(p->suiv)){return;}
		else if (t->section==INI && temp->typ==DIRECTIVE){;}
		else if(current_l-temp->nl==0 && !liste_vide(p->suiv)){
			p=p->suiv;
			temp=p->val;}
		else{;}}
	free(d);
	free(p);
	free(t);}
