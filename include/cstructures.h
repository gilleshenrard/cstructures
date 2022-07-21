#ifndef CSTRUCTURES_H_INCLUDED
#define CSTRUCTURES_H_INCLUDED
#include "carrays.h"
#include "clists.h"
#include "cavl.h"
#include "cqueues.h"

//enums
typedef enum {COPY, REPLACE} e_listtoarray;

//Array, list and AVL transformation
int listToArray(meta_t* dList, meta_t* dArray, e_listtoarray action);
int arrayToList(meta_t* dArray, meta_t* dList, e_listtoarray action);
int arrayToAVL(meta_t* dArray, meta_t* dAVL, e_listtoarray action);

#endif // CSTRUCTURES_H_INCLUDED
