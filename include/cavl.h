#ifndef CAVL_H_INCLUDED
#define CAVL_H_INCLUDED
#include "cstructurescommon.h"

int offset, offset_max;
#define LG_MAX 36

typedef enum {RIGHT, LEFT} e_rotation;

dyndata_t* insertAVL(meta_t* meta, dyndata_t* avl, void* toAdd);
void display_AVL_tree(meta_t* meta, dyndata_t* avl, char dir, char* (*toString)(void*), int (*doPrint)(const char*, ...));
dyndata_t* rotate_AVL(dyndata_t* avl, e_rotation side);
int get_AVL_balance(dyndata_t* avl);
int foreachAVL(meta_t* meta, dyndata_t* avl, void* parameter, int (*doAction)(void*, void*));
void* search_AVL(meta_t* meta, dyndata_t* avl, void* key);
dyndata_t* delete_AVL(meta_t* meta, dyndata_t* root, void* key);
dyndata_t* min_AVL_value(dyndata_t* avl);
int delete_AVL_root(meta_t* meta);

#endif // CAVL_H_INCLUDED
