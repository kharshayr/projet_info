
/**
 * @file lex.c
 * @author François Portet <francois.portet@imag.fr>
 * @brief Lexical analysis routines for MIPS assembly syntax.
 *
 * These routines perform the analysis of the lexeme of an assembly source code file.
 */

#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include <global.h>
#include <notify.h>
#include <lex.h>
#include <liste.h>
#include <assemblage.h>

void affiche(void* e) {
    static char* mesval[] = {"COMMENT", "NL", "SYMBOLE", "DEUX_PTS", "VIRGULE", "DEB_OFFSETBASE", "OFFSETBASE", "DIRECTIVE", "REGISTRE", "DECIMAL", "HEXA\t", "ZERO", "CHAINE\t", "INIT", "ERREUR"};
    printf("[%s\t]\t%s\t\t\t Ligne : %i\n",mesval[((lexeme*)e)->typ],((lexeme*)e)->tok, ((lexeme*)e)->nl);
}

void affiche_erreurs_lex(Liste l){
      printf("Erreur(s) lexicale(s) :\n");
      while (!liste_vide(l->suiv)) {
      	if (((lexeme*)(l->val))->typ==ERREUR) affiche(l->val);
	    l=l->suiv;}
}

lexeme* init_lex(char* token, int n) {
    lexeme* l = calloc(1,sizeof(*l));
    l->tok=calloc(1,(strlen(token)+1)*sizeof(char));
    strcpy(l->tok,token);
    l->typ=INIT;
    l->nl=n;
    return l;
}

void lex_fsm(lexeme* lex) {
    int i=0;
    char* c;
    c=lex->tok;

    while (c[i]!='\0') {

        switch (lex->typ) {

        case INIT:
            if(isdigit(c[i])) {
                lex->typ = (c[i]=='0') ? ZERO : DECIMAL;
            }
            else if (ispunct(c[i])) {
                switch (c[i]) {
                case '.':
                    lex->typ = DIRECTIVE;
                    break;
                case ',':
                    lex->typ = VIRGULE;
                    break;
                case ':':
                    lex->typ = DEUX_PTS;
                    break;
                case '$':
                    lex->typ = REGISTRE;
                    break;
                case '#': /* Les comentaires sont préparés en dehors de la fonction,il suffit de lire le premier caractère */
                    lex->typ = COMMENT;
                    return;
                    break;
                case '"': /* Pareil pour les chaines */
                    lex->typ = CHAINE;
                    return;
                    break;
                case '(':
                    lex->typ = DEB_OFFSETBASE;
                    break;
                }
            }
            else if (isalpha(c[i])) {
                lex->typ = SYMBOLE;
            }
            else {
                lex->typ = ERREUR;
                return;
            }
            break;

        case ZERO:
            if(c[i]=='x'||c[i]=='X') {
                lex->typ = HEXA;
            }
            else if (isdigit(c[i])||c[i]=='\0') lex->typ=DECIMAL;
            else {
                lex->typ = ERREUR;
                return;
            }
            break;

        case DECIMAL:
            if(isdigit(c[i])) lex->typ = DECIMAL;
            else if(c[i]=='(') lex->typ = DEB_OFFSETBASE;
            else {
                lex->typ = ERREUR;
                return;
            }
            break;

        case HEXA:
            if(isxdigit(c[i])) lex->typ = HEXA;
            else if(c[i]=='(') lex->typ = DEB_OFFSETBASE;
            else {
                lex->typ = ERREUR;
                return;
            }
            break;

        case REGISTRE:
            if((i<3) || !strcmp(c,"$zero")) { /* Registre : au maximum 2 alphanum, ou $zero) */
                if(isalnum(c[i])) lex->typ = REGISTRE;
            }
            else {
                lex->typ = ERREUR;
                return;
            }
            break;

        case DIRECTIVE:
            if(isalpha(c[i])) lex->typ = DIRECTIVE;
            else {
                lex->typ = ERREUR;
                return;
            }
            break;

        case DEB_OFFSETBASE:
            if(c[i]=='$' && isalnum(c[i+1])) {
              lex->typ = OFFSETBASE;
            }
            else {
              lex->typ = ERREUR;
              return;
            }
            break;

        case OFFSETBASE:
            if(c[i+1]=='\0' && c[i] != ')') {
              lex->typ = ERREUR;
              return;
            }

        default:
            break;
        }
        i++;
    }
}

Liste lex_read_line( char *line, int nline, Liste lp) {
    char *seps = " \t";
    char *token = NULL;
    char save[STRLEN];

    char* temp = calloc(1,200*sizeof(char)); /* Chaine temporaire pour la construction de commentaires/chaines*/
    int com=0;
    int cdc=0;

    /* copy the input line so that we can do anything with it without impacting outside world*/
    memcpy( save, line, STRLEN );

    /* get each token*/
    for( token = strtok( line, seps ); NULL != token; token = strtok( NULL, seps )) {

        if(token[0] == '#'||com==1) { /* Stockage commentaire */
            if(token[0] == '#') {
                com=1;
                strcat(temp,token);
            }
            else {
                strcat(temp," ");
                strcat(temp,token);
            }
        }

        if ((token[0]=='"')||(cdc==1)) { /* Si guillement de debut ou de fin, ou continuation chaine */
            if((token[0]=='"')&&(cdc==0)) { /* si debut chaine, on commence a stocker */
                cdc=1;
                strcat(temp,token);
            }
            else if((token[0]=='"')&&(cdc==1)) {  /* si fin de chaine, on ferme */
                strcat(temp," ");
                strcat(temp,token);
                cdc=2;
                lexeme* L2 = init_lex(temp,nline);
                lex_fsm(L2);
                lp=ajout_queue((void*)L2,lp);
            }
            else { /* si continuation chaine, on stocke */
                strcat(temp," ");
                strcat(temp,token);
            }
        }
        if ((cdc==0)&&(com==0)) { /* si pas de chaine/commentaire (tous les autres cas) */
            lexeme* L = init_lex(token,nline);
            lex_fsm(L);
            lp=ajout_queue((void*)L,lp);
        }
    } /* Sortie de boucle (fin de ligne) */
    if(com==1) { /* On peut alors ajouter le commentaire complet si c'est le cas */
        lexeme* L3 = init_lex(temp,nline);
        lex_fsm(L3);
        lp=ajout_queue((void*)L3,lp);
    }
    /*lp=ajout_queue(init_lex("\n",nline),lp); // ajout de \n à terminer
    //visualiser_liste(lp,&affiche);*/
    return lp;
}

void lex_standardise(char* in, char* out) {                      /* Fonction qui normalise le texte*/
    char* p=in;
    char* q=out;
    int l;
    while (p<in+strlen(in)) {                                /* Premier traitement : on rajoute des espaces autour de touts les caractères spécifiques*/
	if (*p=='"'){
		*q=*p;
		q++;
		p++;
		while (*p!='"'){
			*q=*p;
			p++;
			q++;}
		*q=' ';
		q++;
		*q=*p;}
        else if (*p==':' || *p==',') {                        /* Pour les caractères nécessitant un espace avant et après*/
            *q=' ';
            q++;
            *q=*p;
            q++;
            *q=' ';
        }
        else if (*p=='#' || *p=='.' || *p=='\n') { /* Pour les caratères nécessitant un espace avant seulement*/
            *q=' ';
            q++;
            *q=*p;
        }
        else {
            *q=*p;
        }
        q++;
        p++;
    }
    *q='\0';
    p=in;
    q=out;
    p=strcpy(p,q); /* On copie pour le second traitement*/
    l=strlen(q);
    p++;
    while (p<in+l+1) { /* Second traitement : on retire tout les espaces en trop*/
	if (*p=='"'){
		q++;
		*q=*p;
		p++;
		q++;
		while (*q!='"'){
			*q=*p;
			p++;
			q++;}
		*q=*p;}
        else if (isblank((int) *p) && *q!=' ') { /* On rajoute un espace si on en a pas encore rajouté un*/
            q++;
            *q=' ';
        }
        else if (isblank((int) *p) && *q==' ') {} /* On passe juste a la suite si on a déjà mis un espace*/
        else if (!isblank((int) *p) && *q==' ') { /* Si il n'y a plus d'espaces à supprimer, on recopie*/
            q++;
            *q=*p;
        }
        else { /* Sinon on recopie simplement*/
            q++;
            *q=*p;}
        p++;}
    *q='\0';
    return;
}

Liste lex_load_file( char *file, unsigned int *nlines, Liste lp, Liste* origine) {

    FILE *fp   = NULL;
    char line[STRLEN]; /* original source line */
    char res[2*STRLEN];

    fp = fopen( file, "r" );
    if ( NULL == fp ) {
        /*macro ERROR_MSG : message d'erreur puis fin de programme ! */
        ERROR_MSG("Error while trying to open %s file --- Aborts",file);
    }

    *nlines = 0;

    while(!feof(fp)) {

        /*read source code line-by-line */
        if ( NULL != fgets( line, STRLEN-1, fp ) ) {
            line[strlen(line)-1] = '\0';  /* eat final '\n' */
            *origine = ajout_queue(init_assembl(line,*nlines+1),*origine);
            (*nlines)++;
            /*      printf("Ligne %i : %s\n", *nlines, line);*/
            if ( 0 != strlen(line) ) {
                lex_standardise(line,res); /*On normalise le texte*/
                lp=lex_read_line(res,*nlines,lp); /* Machine à état + stockage dans une liste */
            }
        }
    }
    fclose(fp);
    return lp;
}
