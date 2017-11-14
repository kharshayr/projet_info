
/**
 * @file main.c
 * @author François Portet <francois.portet@imag.fr> from François Cayre
 * @brief Main entry point for MIPS assembler.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


#include <global.h>
#include <notify.h>
#include <lex.h>
#include <liste.h>
#include <symb.h>
#include <dico.h>

/**
 * @param exec Name of executable.
 * @return Nothing.
 * @brief Print usage.
 *
 */
void print_usage( char *exec ) {
    fprintf(stderr, "Usage: %s file.s\n",
            exec);
}



/**
 * @param argc Number of arguments on the command line.
 * @param argv Value of arguments on the command line.
 * @return Whether this was a success or not.
 * @brief Main entry point for MIPS assembler.
 *
 */
int main ( int argc, char *argv[] ) {

    unsigned int 	nlines 	= 0;
    char         	 *file 	= NULL;

    /* exemples d'utilisation des macros du fichier notify.h */
    /* WARNING_MSG : sera toujours affiche */
    WARNING_MSG("Un message WARNING_MSG !");

    /* macro INFO_MSG : uniquement si compilé avec -DVERBOSE. Cf. Makefile*/
    INFO_MSG("Un message INFO_MSG : Debut du programme %s", argv[0]);

    /* macro DEBUG_MSG : uniquement si compilé avec -DDEBUG (ie : compilation avec make debug). Cf. Makefile */
    DEBUG_MSG("Un message DEBUG_MSG !");

    /* La macro suivante provoquerait l'affichage du message
       puis la sortie du programme avec un code erreur non nul (EXIT_FAILURE) */
    /* ERROR_MSG("Erreur. Arret du programme"); */


    if ( argc <2 ) {
        print_usage(argv[0]);
        exit( EXIT_FAILURE );
    }

    file  	= argv[argc-1];


    if ( NULL == file ) {
        fprintf( stderr, "Missing ASM source file, aborting.\n" );
        exit( EXIT_FAILURE );
    }

    /* ---------------- do the lexical analysis -------------------*/
    Liste l = creer_liste();
    l=lex_load_file(argv[1],&nlines,l);
    DEBUG_MSG("source code got %d lines",nlines);
    visualiser_liste(l,&affiche);
    affiche_erreurs_lex(l);

    /* ---------------- do the syntactic analysis -------------------*/

    symb* s=calloc(N,sizeof(symb));

    Liste* data_l=init_liste();
    Liste* text_l=init_liste();
    Liste* bss_l=init_liste();

    tabl_symb(l,s,data_l,text_l,bss_l);

    affiche_erreurs_lex(l);

    printf("\n-------------------------Collection Symboles-----------------------------\n");
    affiche_tab(s);
    printf("\n----------------------------Collection DATA-----------------------------------\n");
    visualiser_liste(*data_l,&affiche_col);
    printf("\n----------------------------Collection TEXT-----------------------------------\n");
    visualiser_liste(*text_l,&affiche_col);
    printf("\n----------------------------Collection BSS-----------------------------------\n");
    visualiser_liste(*bss_l,&affiche_col);

    free(s);

    /* Chargement du dictionnaire*/
    int nb_inst = 29;
        inst_def_t * tab;
        tab=lect_dico_int("tests/dico.s", &nb_inst);
        int i;
        for (i=0;i<29;i++) {
          printf("%s %c %d\n", tab[i].symbole, tab[i].type, tab[i].nb_op);
        }
    /* A faire : Verification des opérandes des instructions */

    /* ---------------- Free memory and terminate -------------------*/

    /* TODO free everything properly*/

    exit( EXIT_SUCCESS );
}
