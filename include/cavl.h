#ifndef CAVL_H_INCLUDED
#define CAVL_H_INCLUDED
#include "cstructurescommon.h"

//enum used in AVL rotation
typedef enum {
    RIGHT, LEFT
} e_rotation;

//AVL manipulation
dyndata_t* insertAVL(meta_t* meta, dyndata_t* avl, void* toAdd);
int delete_AVL_root(meta_t* meta);

//display function (used in debugging)
void display_AVL_tree(meta_t* meta, dyndata_t* avl, char dir, char* (*toString)(void*), int (*doPrint)(const char*, ...));

//search algorithms
void* search_AVL(meta_t* meta, dyndata_t* avl, void* key);
dyndata_t* min_AVL_value(dyndata_t* avl);

//AVL functor
int foreachAVL(meta_t* meta, dyndata_t* avl, void* parameter, int (*doAction)(void*, void*));

#endif // CAVL_H_INCLUDED
