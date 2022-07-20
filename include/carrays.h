#ifndef CARRAYS_H_INCLUDED
#define CARRAYS_H_INCLUDED
#include "cstructurescommon.h"
#include <inttypes.h>

typedef enum {FIRST, ANY} e_search;

void* get_arrayelem(meta_t* meta, uint32_t i);
int set_arrayelem(meta_t* meta, uint32_t i, void* elem);
int empty_array(meta_t* meta);

int bubbleSortArray(meta_t*, uint32_t);
uint32_t quickSortPartitioning(meta_t*, uint32_t, uint32_t);
int quickSortArray(meta_t*, uint32_t, uint32_t);

int binarySearchArray(meta_t*, void*, e_search scope);

int foreachArray(meta_t*, void*, int (*doAction)(void*, void*));


#endif // CARRAYS_H_INCLUDED
