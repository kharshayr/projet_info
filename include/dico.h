#ifndef _DICO_H_
#define _DICO_H_

#include <stdio.h>
#include <notify.h>

typedef struct { char * symbole; char type ; int nb_op ;} inst_def_t;

inst_def_t * lect_dico_int(char*, int*);

#endif /* _DICO_H_ */
