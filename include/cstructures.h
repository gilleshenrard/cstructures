#ifndef CSTRUCTURES_H_INCLUDED
#define CSTRUCTURES_H_INCLUDED
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>

/*
* NOTE about data structures:
* when dealing with arrays, <meta_t>.structure can direcly be set with a previously generated array.
* when dealing with AVL trees or dynamic lists, dyndata_t elements will be automatically generated,
*       and are not to be handled by the user
*
* NOTE about error printing:
* if doPError is set as NULL, error printing will be ignored
*/

typedef struct dyndata_t dyndata_t;

struct dyndata_t
{
    void*       data;
    int         height;
    dyndata_t*  left;
    dyndata_t*  right;
};

typedef struct
{
    void*       structure;                      //pointer to the root of the whole structure
    dyndata_t*  last;                           //highest element in the structure
    uint32_t    nbelements;                     //number of elements in the structure
    uint32_t    elementsize;                    //size of a core element (actual data without algorithmic overlay)
    int         (*doCompare)(void*, void*);     //comparison method
    void        (*doPError)(char* msg, ...);    //error printing method (can be ignored if set as NULL)
} meta_t;

// variables to use in display_AVL_tree()
int offset, offset_max;
#define LG_MAX 36

//enums
typedef enum {COPY, REPLACE} e_listtoarray;
typedef enum {RIGHT, LEFT} e_rotation;
typedef enum {FIRST, ANY} e_search;

//miscellaneous
dyndata_t* allocate_dyn(meta_t* meta, const void* elem);
int free_dyn(dyndata_t* elem);
int swap_dyn(dyndata_t* a, dyndata_t* b);
void* getdata(dyndata_t* cur);
dyndata_t* getright(dyndata_t* cur);

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
void display_AVL_tree(meta_t* meta, dyndata_t* avl, char dir, char* (*toString)(void*));
dyndata_t* rotate_AVL(dyndata_t* avl, e_rotation side);
int get_AVL_balance(dyndata_t* avl);
int foreachAVL(meta_t* meta, dyndata_t* avl, void* parameter, int (*doAction)(void*, void*));
void* search_AVL(meta_t* meta, dyndata_t* avl, void* key);
dyndata_t* delete_AVL(meta_t* meta, dyndata_t* root, void* key);
dyndata_t* min_AVL_value(dyndata_t* avl);
int delete_AVL_root(meta_t* meta);

#endif // CSTRUCTURES_H_INCLUDED
