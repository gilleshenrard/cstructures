#ifndef CLISTS_H_INCLUDED
#define CLISTS_H_INCLUDED
#include "cstructurescommon.h"
#include <inttypes.h>

//Lists manipulations
int insertListTop(meta_t*, void*);
int insertListBottom(meta_t* meta, void *toAdd);
int popListTop(meta_t*);
int popListBottom(meta_t* meta);
int insertListSorted(meta_t*,  void*);
int removeListSorted(meta_t*, void*);
int freeDynList(meta_t* meta);

//Sorting algorithms
int bubbleSortList(meta_t*, uint32_t);

//Search algorithms
void* get_listelem(meta_t* meta, uint32_t i);

//Lists functor
int foreachList(meta_t*, void*, int (*doAction)(void*, void*));

#endif // CLISTS_H_INCLUDED
