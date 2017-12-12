#ifndef _ASSEMBLAGE_H_
#define _ASSEMBLAGE_H_

#include <stdio.h>
#include <notify.h>

typedef struct assembl_s {
  int num_ligne;
  int code_ligne;
  char* ligne;
} assembl;

assembl* init_assembl();
Liste creer_liste_assemblage(Liste, Liste, Liste, char**);


#endif
