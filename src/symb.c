#define N 100

#include<symb.h>

char * strdup(const char *str){
    int n = strlen(str) + 1;
    char *dup = malloc(n);
    if(dup){
        strcpy(dup, str);}
    return dup;}

Liste pseudo_instr(Liste p, int nl){
	lexeme* temp=p->val;
	strtoupper(temp->tok);
	if (strcmp(temp->tok,"NOP")==0){
		Liste p_memo=p->suiv; /* On mémorise le pointeur sur la sssuite de la chaine */
		temp->tok=strdup("SLL"); /* On remplace NOP par SLL */
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup("$0");temp->typ=REGISTRE;temp->nl=nl; /* On créé le token $0 */
		p->suiv=calloc(1,sizeof(*p));
		p=p->suiv;p->val=temp;
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup("$0");temp->typ=REGISTRE;temp->nl=nl;
		p->suiv=calloc(1,sizeof(*p));
		p=p->suiv;p->val=temp;
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup("0");temp->typ=DECIMAL;temp->nl=nl;
		p->suiv=calloc(1,sizeof(*p));
		p=p->suiv;p->val=temp;
		p->suiv=p_memo; /* On referme la chaine */
		return p_memo;}
	else if (strcmp(temp->tok,"MOVE")==0){ /* Regarder comment LI fonctionne */
		Liste p_memo;
		temp->tok=strdup("ADD");
		if (((lexeme*)(p->suiv->val))->typ==REGISTRE && !liste_vide(p->suiv)){
			p=p->suiv;}
		else{
			return p;}
		if (((lexeme*)(p->suiv->val))->typ==VIRGULE && !liste_vide(p->suiv)){
			p=p->suiv;}
		else{
			return p;}
		if (((lexeme*)(p->suiv->val))->typ==REGISTRE && !liste_vide(p->suiv)){
			p=p->suiv;
			p_memo=p->suiv;}
		else{
			return p;}
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup("$zero");temp->typ=REGISTRE;temp->nl=nl;
		p->suiv=calloc(1,sizeof(*p));
		p=p->suiv;p->val=temp;
		p->suiv=p_memo;
		return p_memo;}
	else if (strcmp(temp->tok,"NEG")==0){ /* Regarder comment LI fonctionne */
		Liste p_memo;
		temp->tok=strdup("SUB");
		if (((lexeme*)(p->suiv->val))->typ==REGISTRE && !liste_vide(p->suiv)){
			p=p->suiv;}
		else{
			return p;}
		if (((lexeme*)(p->suiv->val))->typ==VIRGULE && !liste_vide(p->suiv)){
			p=p->suiv;}
		else{
			return p;}
		if (((lexeme*)(p->suiv->val))->typ==REGISTRE && !liste_vide(p->suiv)){
			p_memo=p->suiv;}
		else{
			return p;}
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup("$zero");temp->typ=REGISTRE;temp->nl=nl;
		p->suiv=calloc(1,sizeof(*p));
		p=p->suiv;p->val=temp;
		p->suiv=p_memo;
		return p_memo;}
	else if (strcmp(temp->tok,"LI")==0){
		Liste p_memo;
		temp->tok=strdup("ADDI"); /* On remplace le token LI par ADDI */
		if (((lexeme*)(p->suiv->val))->typ==REGISTRE && !liste_vide(p->suiv)){ /* On verifie la validité des arguments */
			p=p->suiv;}
		else{
			return p;}
		if (((lexeme*)(p->suiv->val))->typ==VIRGULE && !liste_vide(p->suiv)){
			p=p->suiv;}
		else{
			return p;}
		if ((((lexeme*)(p->suiv->val))->typ==DECIMAL || ((lexeme*)(p->suiv->val))->typ==HEXA) && !liste_vide(p->suiv)){
			p_memo=p->suiv;} /* On memorise le pointeur sur l'immediate */
		else{
			return p;}
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup("$zero");temp->typ=REGISTRE;temp->nl=nl; /* On créé le token $zero */
		p->suiv=calloc(1,sizeof(*p));
		p=p->suiv;p->val=temp;
		p->suiv=p_memo;
		return p_memo;}
	else if (strcmp(temp->tok,"BLT")==0){
		Liste p_memo=p->suiv,p_pts; /* On mémorise le token tu premier registre */
		temp->tok=strdup("SLT"); /* On remplace le token BLT par SLT */
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup("$1");temp->typ=REGISTRE;temp->nl=nl; /* On créé le lexeme suivant */
		p->suiv=calloc(1,sizeof(*p));
		p=p->suiv;p->val=temp;
		p->suiv=p_memo; /* On repointe vers le premier registre */
		if (((lexeme*)(p->suiv->val))->typ==REGISTRE && !liste_vide(p->suiv)){ /* If évaluant la validité des types des arguments donnés */
			p=p->suiv;}
		else{
			return p;}
		if (((lexeme*)(p->suiv->val))->typ==VIRGULE && !liste_vide(p->suiv)){
			p=p->suiv;}
		else{
			return p;}
		if (((lexeme*)(p->suiv->val))->typ==REGISTRE && !liste_vide(p->suiv)){
			p=p->suiv;} /* On pointe sur rs maintenant */
		else{
			return p;}
		if (((lexeme*)(p->suiv->val))->typ==VIRGULE && !liste_vide(p->suiv)){
			p=p->suiv;}
		else{
			return p;}
		if ((((lexeme*)(p->suiv->val))->typ==DECIMAL || ((lexeme*)(p->suiv->val))->typ==HEXA || ((lexeme*)(p->suiv->val))->typ==SYMBOLE) && !liste_vide(p->suiv)){
			p_memo=p->suiv;} /* On mémorise l'adresse de la target */
		else{
			return p;}
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup(":");temp->typ=DEUX_PTS;temp->nl=nl;
		p->suiv=calloc(1,sizeof(*p));
		p=p->suiv;p->val=temp;
		p_pts=p;
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup("BNE");temp->typ=SYMBOLE;temp->nl=nl;
		p->suiv=calloc(1,sizeof(*p));
		p=p->suiv;p->val=temp;
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup("$1");temp->typ=REGISTRE;temp->nl=nl;
		p->suiv=calloc(1,sizeof(*p));
		p=p->suiv;p->val=temp;
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup("$zero");temp->typ=REGISTRE;temp->nl=nl;
		p->suiv=calloc(1,sizeof(*p));
		p=p->suiv;p->val=temp;
		p->suiv=p_memo; /* On referme la chaine */
		return p_pts;}
	else if (strcmp(temp->tok,"LW")==0){
		Liste p_pts;
		if(!liste_vide(p->suiv) && !liste_vide(p->suiv->suiv) && !liste_vide(p->suiv->suiv->suiv)){
		if (((lexeme*)p->suiv->val)->typ==REGISTRE && ((lexeme*)p->suiv->suiv->suiv->val)->typ==SYMBOLE){
		Liste p_next=p->suiv->suiv->suiv->suiv,p_reg=p->suiv,p_eti=p->suiv->suiv->suiv;
		temp->tok=strdup("LUI"); /* On remplace le token LW par LUI*/
		temp=calloc(1,sizeof(lexeme)); /* On créé le prochain token à écrire dans la liste */
		temp->tok=strdup("$at");temp->typ=REGISTRE;temp->nl=nl; /* On remplis le token */
		p->suiv=calloc(1,sizeof(*p));
		p=p->suiv;p->val=temp; /* On ajoute le maillon a la chaine */
		p->suiv=p_eti; /* Prochain lexeme sera etiquette */
		p=p->suiv;
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup(":");temp->typ=DEUX_PTS;temp->nl=nl;
		p->suiv=calloc(1,sizeof(*p));
		p=p->suiv;p->val=temp;
		p_pts=p;
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup("LW");temp->typ=SYMBOLE;temp->nl=nl;
		p->suiv=calloc(1,sizeof(*p));
		p=p->suiv;p->val=temp;
		p->suiv=p_reg; /* Prochain lexeme sera le registre */
		p=p->suiv;
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup(",");temp->typ=VIRGULE;temp->nl=nl; /* On ajoute une virgule pour pas que LW repasse dans la boucle de pseudo instruction */
		p->suiv=calloc(1,sizeof(*p));
		p=p->suiv;p->val=temp;
		temp=calloc(1,sizeof(*temp));
		temp->tok=calloc(512,sizeof(char*));
		temp->tok=strcpy(temp->tok,((lexeme*)p_eti->val)->tok); /* On recopie l'étiquette fournie en argument en tant qu'offset */
		temp->tok=strcat(temp->tok,"($at)"); /* On y rajoute la base */
		temp->typ=OFFSETBASE;temp->nl=nl;
		p->suiv=calloc(1,sizeof(*p));
		p=p->suiv;p->val=temp;
		p->suiv=p_next; /* On referme la chaine */
		return p_pts;}}}
	else if (strcmp(temp->tok,"SW")==0){
		Liste p_pts;
		if(!liste_vide(p->suiv) && !liste_vide(p->suiv->suiv) && !liste_vide(p->suiv->suiv->suiv)){
		if (((lexeme*)p->suiv->val)->typ==REGISTRE && ((lexeme*)p->suiv->suiv->suiv->val)->typ==SYMBOLE){
		Liste p_next=p->suiv->suiv->suiv->suiv,p_reg=p->suiv,p_eti=p->suiv->suiv->suiv;
		temp->tok=strdup("LUI"); /* On remplace le token SW par LUI*/
		temp=calloc(1,sizeof(lexeme)); /* On créé le prochain token à écrire dans la liste */
		temp->tok=strdup("$at");temp->typ=REGISTRE;temp->nl=nl; /* On remplis le token */
		p->suiv=calloc(1,sizeof(*p));
		p=p->suiv;p->val=temp; /* On ajoute le maillon a la chaine */
		p->suiv=p_eti; /* Prochain lexeme sera etiquette */
		p=p->suiv;
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup(":");temp->typ=DEUX_PTS;temp->nl=nl;
		p->suiv=calloc(1,sizeof(*p));
		p=p->suiv;p->val=temp;
		p_pts=p;
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup("SW");temp->typ=SYMBOLE;temp->nl=nl;
		p->suiv=calloc(1,sizeof(*p));
		p=p->suiv;p->val=temp;
		p->suiv=p_reg; /* Prochain lexeme sera le registre */
		p=p->suiv;
		temp=calloc(1,sizeof(lexeme));
		temp->tok=strdup(",");temp->typ=VIRGULE;temp->nl=nl; /* On ajoute une virgule pour pas que LW repasse dans la boucle de pseudo instruction */
		p->suiv=calloc(1,sizeof(*p));
		p=p->suiv;p->val=temp;
		temp=calloc(1,sizeof(lexeme));
		temp->tok=calloc(512,sizeof(char*));
		temp->tok=strcpy(temp->tok,((lexeme*)p_eti->val)->tok); /* On recopie l'étiquette fournie en argument en tant qu'offset */
		temp->tok=strcat(temp->tok,"($at)"); /* On y rajoute la base */
		temp->typ=OFFSETBASE;temp->nl=nl;
		p->suiv=calloc(1,sizeof(*p));
		p=p->suiv;p->val=temp;
		temp=calloc(1,sizeof(lexeme));
		p->suiv=p_next; /* On referme la chaine */
		return p_pts;}}}
	return p;
	}

void affiche_tab(symb* s){ /* Affiche uniquement les etiquettes stockées */
	symb* t=s;
	char* secti[]= {"bss", "data", "text", "INI" ,"undefined"};
	while(t!=s+N){
	if(t->lex.tok!=0){
		printf("On a stocké l'étiquette %s de decalage %d dans la section %s \n",t->lex.tok,t->deca,secti[t->section]);}
		t++;}}

void affiche_col(void* e){ /* Affiche les listes d'instruction constitutées */
	char* secti[]= {"bss", "data", "text", "INI","undefined"};
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

void ajout_liste(Liste* l, Liste p,int t,int* d){ /* Fonction qui allonge et rajoute un élément a la liste */
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

int to_decimal(char* hexa){
	int deca=0;
	int i,n=strlen(hexa);
	int q=1;
	char signe=*hexa;
	if (signe=='0'){
		strtoupper(hexa);
		for (i=0;i<n-2;i++){
			if ((hexa)[n-1-i]<57){
				deca+=(*((hexa)+n-i-1)-48)*q;}
			else{
				deca+=(*((hexa)+n-i-1)-55)*q;}
			q=q*16;}}
	else if (signe=='+' || signe=='-'){
		strtoupper(hexa);
		if (*(hexa+1) >='1' && *(hexa+1)<='9') {
			return 0;}
		for (i=0;i<n-3;i++){
			if ((hexa)[n-1-i]<57){
				deca+=(*((hexa)+n-i-1)-48)*q;}
			else{
				deca+=(*((hexa)+n-i-1)-55)*q;}
			q=q*16;}}
	else if (signe >='1' && signe<='9') {
		return 0;}
	else{
		return 1;}
	if (signe=='-'){
		*hexa=signe;
		sprintf(hexa+1,"%d",deca);}
	else{
		sprintf(hexa,"%d",deca);}
	return 0;}

void tabl_symb(Liste l, symb* s, Liste* data_l, Liste* text_l, Liste* bss_l){
	Liste pseudo;
	int* d=calloc(3,sizeof(int)); /* Stockage et initialisation des decalages [bss,data,text] */
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
					if (strcmp(temp->tok,".bss")==0){t->section=bss;} /* On scanne le token de section et on l'affecte dans le symbole */
					else if (strcmp(temp->tok,".data")==0){t->section=data;}
					else if (strcmp(temp->tok,".text")==0){t->section=text;}
					else{
						temp->typ=ERREUR;
						p=p->suiv;if (!liste_vide(p)){temp=p->val;}} /* ERREUR, on peut pas mettre une autre directive que ces trois là */
					break;
				case COMMENT:
					p=p->suiv;if (!liste_vide(p)){temp=p->val;} /* Si commentaire on passe a la suite */
					break;
				default:
					temp->typ=ERREUR;
					p=p->suiv;if (!liste_vide(p)){temp=p->val;} /* ERREUR, on veut forcément un COMMENT ou une DIRECTIVE */
					break;}
			break;
		case bss:
			switch (temp->typ){
				case DIRECTIVE:
					if (strcmp((temp->tok),".bss")==0){
						p=p->suiv;if (!liste_vide(p)){temp=p->val;}} /* On change pas de section */
					else if (strcmp(temp->tok,".data")==0){init_symb(t);} /* On repasse a l'état INIT */
					else if (strcmp(temp->tok,".text")==0){init_symb(t);}
					else if (strcmp(temp->tok,".space")==0 && !liste_vide(p->suiv)){
						if(((lexeme*)p->suiv->val)->typ==DECIMAL || ((lexeme*)p->suiv->val)->typ==VIRGULE){ /* On vérifie qu'il y est bien au moins un argument */
							ajout_liste(bss_l,p,t->section,d); /* On ajoute le .space à la collection */
							p=p->suiv;if (!liste_vide(p)){temp=p->val;}
							while ((current_l-temp->nl && !liste_vide(p))==0){
								switch(temp->typ){
									case DECIMAL: /* Test validité arguments */
										if (atof(temp->tok)<=4294967295 && atof(temp->tok)>=0){
											ajout_liste(bss_l,p,t->section,d);
											d[bss]=d[bss]+atoi(temp->tok);}
										else{
											temp->typ=ERREUR;}
										break;
									case HEXA:
										to_decimal(temp->tok); /* On convertit l'HEXA en DECIMAL */
										temp->typ=DECIMAL;
										if (atof(temp->tok)<=4294967295 && atof(temp->tok)>=0){
											ajout_liste(bss_l,p,t->section,d);
											d[bss]=d[bss]+atoi(temp->tok);}
										else{
											temp->typ=ERREUR;}
									case VIRGULE:
										break;
									default:
										temp->typ=ERREUR; /* ERREUR, on attend que VIRGULE et DECIMAL en argument */
										break;}
								p=p->suiv;if (!liste_vide(p)){temp=p->val;}}}
						else{
							temp->typ=ERREUR; /* Si on a pas d'argument, ERREUR */
							p=p->suiv;if (!liste_vide(p)){temp=p->val;}}}
					else{
						temp->typ=ERREUR;
						p=p->suiv;if (!liste_vide(p)){temp=p->val;}} /* ERREUR, pas d'autres directives acceptées dans .bss */
					break;
				case COMMENT:
					p=p->suiv;if (!liste_vide(p)){temp=p->val;} /* On ignore les commentaires */
					break;
				case SYMBOLE:
					if (!liste_vide(p->suiv)){ /* Si jamais on définit une étiquette dans bss */
						if (((lexeme*)p->suiv->val)->typ==DEUX_PTS){
							t->lex=*temp;
							t->deca=d[bss];
							ajout_tab(s,t); /* On l'ajoute a la table des symboles */
							p=p->suiv;p=p->suiv;if (!liste_vide(p)){temp=p->val;}}
						else{
							temp->typ=ERREUR;
							p=p->suiv;if (!liste_vide(p)){temp=p->val;}}} /* ERREUR, on veut pas d'instruction */
					break;
				default:
					temp->typ=ERREUR;
					p=p->suiv;if (!liste_vide(p)){temp=p->val;} /* ERREUR, on attend que DIRECTIVE ou COMMENT ou ETIQUETTE dans cette section */
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
									to_decimal(temp->tok);
									temp->typ=DECIMAL;
									if (atof(temp->tok)<=4294967295 && atof(temp->tok)>=0){
										if (d[data]%4!=0){
											d[data]=4+d[data]-d[data]%4;}
										ajout_liste(data_l,p,t->section,d);
										d[data]=d[data]+4;}
									else{
										temp->typ=ERREUR;}
								break;
								case DECIMAL:
									if (atof(temp->tok)<=4294967295 && atof(temp->tok)>=0){
										if (d[data]%4!=0){
											d[data]=4+d[data]-d[data]%4;}
										ajout_liste(data_l,p,t->section,d);
										d[data]=d[data]+4;}
									else{
										temp->typ=ERREUR;}
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
									temp->typ=ERREUR; /* ERREUR, on attend un DECIMAL ou un HEXA */
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
										if (atof(temp->tok)<=255 && atof(temp->tok)>=0){
											ajout_liste(data_l,p,t->section,d);
											d[data]=d[data]+1;}
										else{
											temp->typ=ERREUR;}
										break;
									case HEXA:
										to_decimal(temp->tok);
										temp->typ=DECIMAL;
										if (atof(temp->tok)<=255 && atof(temp->tok)>=0){
											ajout_liste(data_l,p,t->section,d);
											d[data]=d[data]+1;}
										else{
											temp->typ=ERREUR;}
										break;
									case VIRGULE:
										break;
									default:
										temp->typ=ERREUR; /* ERREUR, on attend que VIRGULE et DECIMAL en argument */
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
										d[data]=d[data]+strlen(temp->tok)-3; /* -3 Du aux espaces rajoutés par standardise + les guillemets*/
										break;
									case VIRGULE:
										break;
									default:
										temp->typ=ERREUR; /* ERREUR, on attend que VIRGULE et DECIMAL en argument */
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
										to_decimal(temp->tok);
										temp->typ=DECIMAL;
										if (atof(temp->tok)<=4294967295 && atof(temp->tok)>=0){
											ajout_liste(bss_l,p,t->section,d);
											d[bss]=d[bss]+atoi(temp->tok);}
										else{
											temp->typ=ERREUR;}
										break;
									case DECIMAL:
										if (atof(temp->tok)<=4294967295 && atof(temp->tok)>=0){
											ajout_liste(data_l,p,t->section,d);
											d[data]=d[data]+atoi(temp->tok);}
										else{
											temp->typ=ERREUR;}
										break;
									case VIRGULE:
										break;
									default:
										temp->typ=ERREUR;
										break;}
								p=p->suiv;if (!liste_vide(p)){temp=p->val;}}}
						else{
							temp->typ=ERREUR;
							p=p->suiv;if (!liste_vide(p)){temp=p->val;}}}
					else{
						temp->typ=ERREUR;
						p=p->suiv;if (!liste_vide(p)){temp=p->val;}}
					break;
				case COMMENT:
					p=p->suiv;if (!liste_vide(p)){temp=p->val;}
					break;
				case SYMBOLE:
					if (!liste_vide(p->suiv)){
						if (((lexeme*)p->suiv->val)->typ==DEUX_PTS){ /* Etiquette définie dans .data */
							t->lex=*temp;
							t->deca=d[data];
							ajout_tab(s,t);
							p=p->suiv;p=p->suiv;if (!liste_vide(p)){temp=p->val;}}}
					else{
						temp->typ=ERREUR;
						p=p->suiv;if (!liste_vide(p)){temp=p->val;} /* ERREUR, on veut pas d'instruction */}
					break;

				default:
					temp->typ=ERREUR;
					p=p->suiv;if (!liste_vide(p)){temp=p->val;} /* ERREUR, on attend que DIRECTIVE ou COMMENT ou ETIQUETTE */
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
						p=p->suiv;if (!liste_vide(p)){temp=p->val;} /* ERREUR on attend aucune DIRECTIVE autre que de section */}
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
							pseudo=pseudo_instr(p,temp->nl);
							while (current_l-temp->nl==0 && !liste_vide(p) && temp->typ!=DEUX_PTS){
								if (temp->typ==HEXA){
									to_decimal(temp->tok);
									temp->typ=DECIMAL;}
								switch (temp->typ){
									case VIRGULE:
										break;
									case COMMENT:
										break;
									default:
										ajout_liste(text_l,p,t->section,d);
										break;}
								p=p->suiv;if (!liste_vide(p)){temp=p->val;}}
							d[text]=d[text]+4;
							if (temp->typ==DEUX_PTS){p=p->suiv;if (!liste_vide(p)){temp=p->val;}}}}
					else{
						temp->typ=ERREUR;
						p=p->suiv;if (!liste_vide(p)){temp=p->val;}}
					break;
				default:
					temp->typ=ERREUR;
					p=p->suiv;if (!liste_vide(p)){temp=p->val;} /* ERREUR, on attend pas autre chose */
					break;}
			break;
		default:
			temp->typ=ERREUR;
			p=p->suiv;if (!liste_vide(p)){temp=p->val;}
			break;}}
	free(d);
	free(p);
	free(t);}
