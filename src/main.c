
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
#include <verif.h>
#include <reloc.h>
#include <assemblage.h>

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
    Liste* origine_l = calloc(1,sizeof(origine_l));
    l=lex_load_file(argv[1],&nlines,l,origine_l);
    DEBUG_MSG("source code got %d lines",nlines);
    visualiser_liste(l,&affiche);
    affiche_erreurs_lex(l);

    /* ---------------- do the syntactic analysis -------------------*/

    symb* s=calloc(N,sizeof(symb));

    Liste* data_l=init_liste();
    Liste* text_l=init_liste();
    Liste* bss_l=init_liste();

    tabl_symb(l,s,data_l,text_l,bss_l);

    affiche_erreurs_gram(l);

    printf("\n-------------------------Collection Symboles-----------------------------\n\n");
    affiche_tab(s);
    printf("\n----------------------------Collection DATA-----------------------------------\n\n");
    visualiser_liste(*data_l,&affiche_col);
    printf("\n----------------------------Collection TEXT-----------------------------------\n\n");
    visualiser_liste(*text_l,&affiche_col);
    printf("\n----------------------------Collection BSS-----------------------------------\n\n");
    visualiser_liste(*bss_l,&affiche_col);

    /* Chargement du dictionnaire*/
    int nb_inst = 25;
    inst_def_t * tab;
    tab=lect_dico_int("tests/dico.s", &nb_inst);

    /* Examen argument instruction data */
    Liste arg_text=verif_arg_text(text_l,tab,nb_inst,s);
    affiche_erreurs_dico(*text_l);
    Liste arg_data=verif_arg_data(data_l);
    affiche_liste_ope_text(arg_text);
    affiche_liste_ope_data(arg_data);

    Liste reloc_t_text = creer_table_reloc(arg_text,s);
    visualiser_liste(reloc_t_text,&affiche_reloc);
    Liste reloc_t_data = creer_table_reloc(arg_data,s);

    printf("\nListe d'assemblage : \n\n");
    calcul_code_assemblage(arg_text, arg_data, *bss_l, origine_l, &nlines, s);
    visualiser_liste(*origine_l,&affiche_assembl);
    


    /* On libère */
    free(tab);
    free(data_l);
    free(text_l);
    free(bss_l);
    free(s);

    exit( EXIT_SUCCESS );
}
