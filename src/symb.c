#define N 100

#include<symb.h>

void pseudo_instr(Liste p, int nl){
	lexeme* temp=p->val;
	Liste r=p->suiv;
	strtoupper(temp->tok);
	if (strcmp(temp->tok,"NOP")==0){
		temp->tok=strdup("SLL");
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup("$0");temp->typ=REGISTRE;temp->nl=nl;
		p->suiv=calloc(1,sizeof(Liste));
		p=p->suiv;
		p->val=temp;
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup("$0");temp->typ=REGISTRE;temp->nl=nl;
		p->suiv=calloc(1,sizeof(Liste));
		p=p->suiv;
		p->val=temp;
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup("0");temp->typ=DECIMAL;temp->nl=nl;
		p->suiv=calloc(1,sizeof(Liste));
		p=p->suiv;
		p->val=temp;
		p->suiv=r;}
	else if (strcmp(temp->tok,"MOVE")==0){
		if (((lexeme*)(p->suiv->val))->typ==REGISTRE){
			p=p->suiv;}
		else{
			((lexeme*)(p->val))->typ==ERREUR;
			return;}
		if (((lexeme*)(p->suiv->val))->typ==VIRGULE){
			p=p->suiv;}
		else{
			((lexeme*)(p->val))->typ==ERREUR;
			return;}
		if (((lexeme*)(p->suiv->val))->typ==REGISTRE){
			p=p->suiv;
			r=p->suiv;}
		else{
			((lexeme*)(p->val))->typ==ERREUR;
			return;}
		temp->tok=strdup("ADD");
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup("$zero");temp->typ=REGISTRE;temp->nl=nl;
		p->suiv=calloc(1,sizeof(Liste));
		p=p->suiv;
		p->val=temp;
		p->suiv=r;}
	else if (strcmp(temp->tok,"NEG")==0){
		if (((lexeme*)(p->suiv->val))->typ!=REGISTRE){((lexeme*)(p->suiv->val))->typ=ERREUR;return;}
		if (((lexeme*)(p->suiv->suiv->val))->typ!=VIRGULE){((lexeme*)(p->suiv->val))->typ=ERREUR;return;}
		if (((lexeme*)(p->suiv->suiv->suiv->val))->typ==REGISTRE){
			r=p->suiv->suiv->suiv->suiv;}
		else{
			((lexeme*)(p->suiv->val))->typ=ERREUR;
			return;}
		p=p->suiv->suiv->suiv;
		temp->tok=strdup("SUB");
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup("$zero");temp->typ=REGISTRE;temp->nl=nl;
		p->suiv=calloc(1,sizeof(Liste));
		p=p->suiv;
		p->val=temp;
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup(",");temp->typ=VIRGULE;temp->nl=nl;
		p->suiv=calloc(1,sizeof(Liste));
		p=p->suiv;
		p->val=temp;
		p->suiv=r;}
	}

void affiche_tab(symb* s){ /* Affiche uniquement les etiquettes stockées */
	symb* t=s;
	char* secti[]= {"bss", "data", "text", "INI"};
	while(t!=s+N){
	if(t->lex.tok!=0){
		printf("On a stocké l'étiquette %s de decalage %d dans la section %s \n",t->lex.tok,t->deca,secti[t->section]);}
		t++;}}

void affiche_col(void* e){ /* Affiche les listes d'instruction constitutées */
	char* secti[]= {"bss", "data", "text", "INI"};
	if(((symb*)e)->lex.typ==DIRECTIVE || ((symb*)e)->lex.typ==SYMBOLE){
		printf("Instruction : %s\t\t Decalage : %d\t\t Section : %s \n",((symb*)e)->lex.tok,((symb*)e)->deca,secti[((symb*)e)->section]);
	}
	else{
		printf("Operande : %s\t\t Decalage : %d\t\t Section : %s \n",((symb*)e)->lex.tok,((symb*)e)->deca,secti[((symb*)e)->section]);
	}
}

void affiche_erreurs_gram(Liste l){
      printf("Erreur(s) gramaticales(s) :\n");
      while (!liste_vide(l)) {
      	if (((lexeme*)(l->val))->typ==ERREUR) affiche(l->val);
	    l=l->suiv;}
}

void ajout_liste(Liste* l, Liste p,int t,int* d){ /*Fonction qui allonge et rajoute un élément a la liste */
	symb* temp=calloc(1,sizeof(symb));
	temp->lex=*((lexeme*)p->val);
	temp->deca=d[t];
	temp->section=t;
	*l=ajout_queue(temp,*l);
}

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

void init_symb(symb* t){ /* Fonction qui renvoie a l'etat initial */
	t->section=INI;}

Liste* init_liste(){
		Liste* l = calloc(1,sizeof(*l));
		*l=creer_liste();
		return l;
}

void to_decimal(lexeme* hexa){
	int deca=0;
	int i,n=strlen(hexa->tok);
	int q=1;
	if (*(hexa->tok)=='0'){
		for (i=0;i<n-2;i++){
			if ((hexa->tok)[n-1-i]<57){
				deca+=(*((hexa->tok)+n-i-1)-48)*q;}
			else{
				deca+=(*((hexa->tok)+n-i-1)-87)*q;}
			q=q*16;}}
	else{
		for (i=0;i<n-2;i++){
			if ((hexa->tok)[n-1-i]<57){
				deca+=(*((hexa->tok)+n-i-1)-48)*q;}
			else{
				deca+=(*((hexa->tok)+n-i-1)-87)*q;}
			q=q*16;}}
	sprintf(hexa->tok,"%d",deca);
	hexa->typ=DECIMAL;}

void tabl_symb(Liste l, symb* s, Liste* data_l, Liste* text_l, Liste* bss_l){
	int* d=calloc(3,sizeof(int));/* Stockage et initialisation des decalages [bss,data,text] */
	Liste p=l;
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
					else{
						temp->typ=ERREUR;
						p=p->suiv;if (!liste_vide(p)){temp=p->val;}}/* ERREUR, on peut pas mettre une autre directive de ces trois là */
					break;
				case COMMENT:
					p=p->suiv;if (!liste_vide(p)){temp=p->val;}
					break;
				default:
					temp->typ=ERREUR;
					p=p->suiv;if (!liste_vide(p)){temp=p->val;}/* ERREUR, on veut forcément un COMMENT ou une DIRECTIVE */
					break;}
			break;
		case bss:
			switch (temp->typ){
				case DIRECTIVE:
					if (strcmp((temp->tok),".bss")==0){
						p=p->suiv;if (!liste_vide(p)){temp=p->val;}} /* On change pas de section */
					else if (strcmp(temp->tok,".data")==0){init_symb(t);}
					else if (strcmp(temp->tok,".text")==0){init_symb(t);}
					else if (strcmp(temp->tok,".space")==0 && !liste_vide(p->suiv)){
						if(((lexeme*)p->suiv->val)->typ==DECIMAL || ((lexeme*)p->suiv->val)->typ==VIRGULE){
							ajout_liste(bss_l,p,t->section,d);
							p=p->suiv;if (!liste_vide(p)){temp=p->val;}
							while ((current_l-temp->nl && !liste_vide(p))==0){
								switch(temp->typ){
									case DECIMAL: /* Test validité arguments */
										ajout_liste(bss_l,p,t->section,d);
										d[bss]=d[bss]+atoi(temp->tok);
										break;
									case HEXA:
										to_decimal(temp);
										ajout_liste(bss_l,p,t->section,d);
										d[bss]=d[bss]+atoi(temp->tok);
									case VIRGULE:
										break;
									default:
										temp->typ=ERREUR;/* ERREUR, on attend que VIRGULE et DECIMAL en argument */
										break;}
								p=p->suiv;if (!liste_vide(p)){temp=p->val;}}}
						else{
							temp->typ=ERREUR;
							p=p->suiv;if (!liste_vide(p)){temp=p->val;}}}
					else{
						temp->typ=ERREUR;
						p=p->suiv;if (!liste_vide(p)){temp=p->val;}}/* ERREUR, pas d'autres directives acceptées dans .bss */
					break;
				case COMMENT:
					p=p->suiv;if (!liste_vide(p)){temp=p->val;}
					break;
				case SYMBOLE:
					if (!liste_vide(p->suiv)){
						if (((lexeme*)p->suiv->val)->typ==DEUX_PTS){
							t->lex=*temp;
							t->deca=d[bss];
							ajout_tab(s,t);
							p=p->suiv;p=p->suiv;if (!liste_vide(p)){temp=p->val;}}
						else{
							temp->typ=ERREUR;
							p=p->suiv;if (!liste_vide(p)){temp=p->val;}}}/* ERREUR, on veut pas d'instruction */
					break;
				default:
					temp->typ=ERREUR;
					p=p->suiv;if (!liste_vide(p)){temp=p->val;}/* ERREUR, on attend que DIRECTIVE ou COMMENT ou ETIQUETTE dans cette section */
					break;}
			break;
		case data:
			switch (temp->typ){
				case DIRECTIVE:
					if (strcmp(temp->tok,".data")==0){
						p=p->suiv;if (!liste_vide(p)){temp=p->val;}}
					else if (strcmp(temp->tok,".bss")==0){init_symb(t);}
					else if (strcmp(temp->tok,".text")==0){init_symb(t);}
					else if (strcmp(temp->tok,".word")==0  && !liste_vide(p->suiv)){
						if(((lexeme*)p->suiv->val)->typ==DECIMAL || ((lexeme*)p->suiv->val)->typ==VIRGULE || ((lexeme*)p->suiv->val)->typ==HEXA){
						ajout_liste(data_l,p,t->section,d);
						p=p->suiv;if (!liste_vide(p)){temp=p->val;}
						while (current_l-temp->nl==0  && !liste_vide(p)){
							switch (temp->typ){
								case HEXA:
									to_decimal(temp);
									if (d[data]%4!=0){
										d[data]=4+d[data]-d[data]%4;}
									ajout_liste(data_l,p,t->section,d);
									d[data]=d[data]+4;
								break;
								case DECIMAL:
									if (d[data]%4!=0){
										d[data]=4+d[data]-d[data]%4;}
									ajout_liste(data_l,p,t->section,d);
									d[data]=d[data]+4;
									break;
								case SYMBOLE:
									if (d[data]%4!=0){
										d[data]=4+d[data]-d[data]%4;}
									ajout_liste(data_l,p,t->section,d);
									d[data]=d[data]+4;
									break;
								case VIRGULE:
									break;

								default:
									temp->typ=ERREUR;/* ERREUR, on attend un DECIMAL ou un HEXA */
									break;}
								p=p->suiv;if (!liste_vide(p)){temp=p->val;}}}
							else{
								temp->typ=ERREUR;
								p=p->suiv;if (!liste_vide(p)){temp=p->val;}}}
					else if (strcmp((temp->tok),".byte")==0  && !liste_vide(p->suiv)){
						if(((lexeme*)p->suiv->val)->typ==DECIMAL || ((lexeme*)p->suiv->val)->typ==VIRGULE || ((lexeme*)p->suiv->val)->typ==HEXA){
							ajout_liste(data_l,p,t->section,d);
							p=p->suiv;if (!liste_vide(p)){temp=p->val;}
							while (current_l-temp->nl==0  && !liste_vide(p)){
								switch(temp->typ){
									case DECIMAL:
										ajout_liste(data_l,p,t->section,d);
										d[data]=d[data]+1;
										break;
									case HEXA:
										to_decimal(temp);
										ajout_liste(data_l,p,t->section,d);
										d[data]=d[data]+1;
										break;
									case VIRGULE:
										break;
									default:
										temp->typ=ERREUR;/* ERREUR, on attend que VIRGULE et DECIMAL en argument */
										break;}
								p=p->suiv;if (!liste_vide(p)){temp=p->val;}}}
						else{
							temp->typ=ERREUR;
							p=p->suiv;if (!liste_vide(p)){temp=p->val;}}}
					else if (strcmp(temp->tok,".asciiz")==0 && !liste_vide(p->suiv)){
						if(((lexeme*)p->suiv->val)->typ==CHAINE || ((lexeme*)p->suiv->val)->typ==VIRGULE){
							ajout_liste(data_l,p,t->section,d);
							p=p->suiv;if (!liste_vide(p)){temp=p->val;}
							while (current_l-temp->nl==0 && !liste_vide(p)){
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
									p=p->suiv;if (!liste_vide(p)){temp=p->val;}}}
						else{
							temp->typ=ERREUR;
							p=p->suiv;if (!liste_vide(p)){temp=p->val;}}}
					else if (strcmp((temp->tok),".space")==0 && !liste_vide(p->suiv)){
						if(((lexeme*)p->suiv->val)->typ==DECIMAL || ((lexeme*)p->suiv->val)->typ==VIRGULE){
							ajout_liste(data_l,p,t->section,d);
							p=p->suiv;if (!liste_vide(p)){temp=p->val;}
							while (current_l-temp->nl==0  && !liste_vide(p)){
								switch(temp->typ){
									case HEXA:
										to_decimal(temp);
										ajout_liste(bss_l,p,t->section,d);
										d[bss]=d[bss]+atoi(temp->tok);
										break;
									case DECIMAL:
										ajout_liste(data_l,p,t->section,d);
										d[data]=d[data]+atoi(temp->tok);
										break;
									case VIRGULE:
										break;
									default:
										temp->typ=ERREUR;/*ERREUR, on attend que VIRGULE et DECIMAL en argument */
										break;}
								p=p->suiv;if (!liste_vide(p)){temp=p->val;}}}
						else{
							temp->typ=ERREUR;
							p=p->suiv;if (!liste_vide(p)){temp=p->val;}}}
					else{
						temp->typ=ERREUR;
						p=p->suiv;if (!liste_vide(p)){temp=p->val;}}/*ERREUR, directive dans mauvaise section */
					break;
				case COMMENT:
					p=p->suiv;if (!liste_vide(p)){temp=p->val;}
					break;
				case SYMBOLE:
					if (!liste_vide(p->suiv)){
						if (((lexeme*)p->suiv->val)->typ==DEUX_PTS){
							t->lex=*temp;
							t->deca=d[data];
							ajout_tab(s,t);
							p=p->suiv;p=p->suiv;if (!liste_vide(p)){temp=p->val;}}}
					else{
						temp->typ=ERREUR;
						p=p->suiv;if (!liste_vide(p)){temp=p->val;}/*ERREUR, on veut pas d'instruction */}
					break;

				default:
					temp->typ=ERREUR;
					p=p->suiv;if (!liste_vide(p)){temp=p->val;}/*ERREUR, on attend que DIRECTIVE ou COMMENT ou ETIQUETTE */
					break;}
			break;
		case text:
			switch(temp->typ){
				case DIRECTIVE:
					if (strcmp(temp->tok,".text")==0){
						p=p->suiv;if (!liste_vide(p)){temp=p->val;}}
					else if (strcmp(temp->tok,".bss")==0){init_symb(t);}
					else if (strcmp(temp->tok,".data")==0){init_symb(t);}
					else {
						temp->typ=ERREUR;
						p=p->suiv;if (!liste_vide(p)){temp=p->val;}/*ERREUR on attend aucune DIRECTIVE autre que de section */}
					break;
				case COMMENT:
					p=p->suiv;if (!liste_vide(p)){temp=p->val;}
					break;
				case SYMBOLE:
					if (!liste_vide(p->suiv)){
						if (((lexeme*)p->suiv->val)->typ==DEUX_PTS){
							t->lex=*temp;
							t->deca=d[text];
							ajout_tab(s,t);
							p=p->suiv;p=p->suiv;if (!liste_vide(p)){temp=p->val;}}
						else{
							pseudo_instr(p,temp->nl);
							while (current_l-temp->nl==0 && !liste_vide(p)){
								if (temp->typ==HEXA){
									to_decimal(temp);}
								switch (temp->typ){
									case VIRGULE:
										break;
									case ERREUR:
										while (current_l-temp->nl==0 && !liste_vide(p)){p=p->suiv;if (!liste_vide(p)){temp=p->val;}}
										break;
									default:
										ajout_liste(text_l,p,t->section,d);
										break;}
								p=p->suiv;if (!liste_vide(p)){temp=p->val;}}
							d[text]=d[text]+4;}}
					else{
						temp->typ=ERREUR;
						p=p->suiv;if (!liste_vide(p)){temp=p->val;}}
					break;
				default:
					temp->typ=ERREUR;
					p=p->suiv;if (!liste_vide(p)){temp=p->val;}/*ERREUR, on attend pas autre chose */
					break;}
			break;
		default:
			temp->typ=ERREUR;
			p=p->suiv;if (!liste_vide(p)){temp=p->val;}
			break;}}
	free(d);
	free(p);
	free(t);}
