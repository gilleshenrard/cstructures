# cstructures
## A simple containers library in C
---
### 1. Intro
This library aims to provide basic OOP containers for C applications.

For now, only vectors (arrays), bidirectional lists and AVL trees are implemented.

Each element of the containers is encased in the dyndata_t structure, and the containers use the meta_t structure to function.

It is heavily based on void pointers and function pointers, thus allowing a maximum flexibility of use.

dyndata_t :
```C
struct dyndata_t
{
    void*       data;				//container element
    int         height;				//height (in AVL trees)
    dyndata_t*  left;				//left element
    dyndata_t*  right;				//right element
};
```

meta_t :
```C
typedef struct
{
    void*       structure;                      //pointer to the root of the whole structure
    dyndata_t*  last;                           //highest element in the structure
    uint32_t    nbelements;                     //number of elements in the structure
    uint32_t    elementsize;                    //size of a core element (actual data without algorithmic overlay)
    int         (*doCompare)(void*, void*);     //comparison method
    void        (*doPError)(char* msg, ...);    //error printing method (can be ignored if set as NULL)
} meta_t;
```

### 2. Makefile
A makefile is provided. It allows the following :
* compiling the code as a static library
* compiling the test program
* cleaning the project directory


### 3. Test program
A test program, bin/test_cstructures, is provided to demonstrate the library features.

### 4. Current features
* Containers transformations (container type to another) :
```C
int listToArray(meta_t* dList, meta_t* dArray, e_listtoarray action);
int arrayToList(meta_t* dArray, meta_t* dList, e_listtoarray action);
int arrayToAVL(meta_t* dArray, meta_t* dAVL, e_listtoarray action);
```

* Arrays :
```C
void* get_arrayelem(meta_t* meta, uint32_t i);
int set_arrayelem(meta_t* meta, uint32_t i, void* elem);
int empty_array(meta_t* meta);
int foreachArray(meta_t*, void*, int (*doAction)(void*, void*));
```

* Arrays sorting algorithms :
```C
int bubbleSortArray(meta_t*, uint32_t);
int bubbleSortList(meta_t*, uint32_t);
uint32_t quickSortPartitioning(meta_t*, uint32_t, uint32_t);
int quickSortArray(meta_t*, uint32_t, uint32_t);
```

* Arrays research algorithms :
```C
int binarySearchArray(meta_t*, void*, e_search scope);
```

* Bidirectional lists :
```C
void* get_listelem(meta_t* meta, uint32_t i);
int insertListTop(meta_t*, void*);
int insertListBottom(meta_t* meta, void *toAdd);
int popListTop(meta_t*);
int popListBottom(meta_t* meta);
int insertListSorted(meta_t*,  void*);
int removeListSorted(meta_t*, void*);
int freeDynList(meta_t* meta);
int foreachList(meta_t*, void*, int (*doAction)(void*, void*));
```

* AVL trees :
```C
dyndata_t* insertAVL(meta_t* meta, dyndata_t* avl, void* toAdd);
void display_AVL_tree(meta_t* meta, dyndata_t* avl, char dir, char* (*toString)(void*), int (*doPrint)(const char*, ...));
dyndata_t* rotate_AVL(dyndata_t* avl, e_rotation side);
int get_AVL_balance(dyndata_t* avl);
int foreachAVL(meta_t* meta, dyndata_t* avl, void* parameter, int (*doAction)(void*, void*));
void* search_AVL(meta_t* meta, dyndata_t* avl, void* key);
dyndata_t* delete_AVL(meta_t* meta, dyndata_t* root, void* key);
dyndata_t* min_AVL_value(dyndata_t* avl);
int delete_AVL_root(meta_t* meta);
```

### 5. To Do
* Implement more features on existing containers
* Implement other containers (queues, stacks, ...)


### 6. Known issues
n/a

