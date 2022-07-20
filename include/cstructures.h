#ifndef CSTRUCTURES_H_INCLUDED
#define CSTRUCTURES_H_INCLUDED
#include "cstructurescommon.h"

// variables to use in display_AVL_tree()
int offset, offset_max;
#define LG_MAX 36

//enums
typedef enum {COPY, REPLACE} e_listtoarray;
typedef enum {RIGHT, LEFT} e_rotation;
typedef enum {FIRST, ANY} e_search;

//Arrays
void* get_arrayelem(meta_t* meta, uint32_t i);
int set_arrayelem(meta_t* meta, uint32_t i, void* elem);
int empty_array(meta_t* meta);

//Array, list and AVL transformation
int listToArray(meta_t* dList, meta_t* dArray, e_listtoarray action);
int arrayToList(meta_t* dArray, meta_t* dList, e_listtoarray action);
int arrayToAVL(meta_t* dArray, meta_t* dAVL, e_listtoarray action);

//Sorting algorithms
int bubbleSortArray(meta_t*, uint32_t);
int bubbleSortList(meta_t*, uint32_t);
uint32_t quickSortPartitioning(meta_t*, uint32_t, uint32_t);
int quickSortArray(meta_t*, uint32_t, uint32_t);

//Research algorithms
int binarySearchArray(meta_t*, void*, e_search scope);

//Dynamic lists
void* get_listelem(meta_t* meta, uint32_t i);
int insertListTop(meta_t*, void*);
int insertListBottom(meta_t* meta, void *toAdd);
int popListTop(meta_t*);
int popListBottom(meta_t* meta);
int insertListSorted(meta_t*,  void*);
int removeListSorted(meta_t*, void*);
int freeDynList(meta_t* meta);
int foreachList(meta_t*, void*, int (*doAction)(void*, void*));

//Arrays
int foreachArray(meta_t*, void*, int (*doAction)(void*, void*));

////AVL trees
dyndata_t* insertAVL(meta_t* meta, dyndata_t* avl, void* toAdd);
void display_AVL_tree(meta_t* meta, dyndata_t* avl, char dir, char* (*toString)(void*), int (*doPrint)(const char*, ...));
dyndata_t* rotate_AVL(dyndata_t* avl, e_rotation side);
int get_AVL_balance(dyndata_t* avl);
int foreachAVL(meta_t* meta, dyndata_t* avl, void* parameter, int (*doAction)(void*, void*));
void* search_AVL(meta_t* meta, dyndata_t* avl, void* key);
dyndata_t* delete_AVL(meta_t* meta, dyndata_t* root, void* key);
dyndata_t* min_AVL_value(dyndata_t* avl);
int delete_AVL_root(meta_t* meta);

#endif // CSTRUCTURES_H_INCLUDED
