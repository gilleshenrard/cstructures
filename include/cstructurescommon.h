#ifndef CSTRUCTURESCOMMON_H_INCLUDED
#define CSTRUCTURESCOMMON_H_INCLUDED
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

//struct holding a container element
struct dyndata_t
{
    void*       data;
    int         height;
    dyndata_t*  left;
    dyndata_t*  right;
};

//container metadata
typedef struct
{
    void*       structure;                      //pointer to the root of the whole structure
    dyndata_t*  last;                           //highest element in the structure
    uint32_t    nbelements;                     //number of elements in the structure
    uint32_t    elementsize;                    //size of a core element (actual data without algorithmic overlay)
    int         (*doCompare)(void*, void*);     //comparison method
    void        (*doPError)(char* msg, ...);    //error printing method (can be ignored if set as NULL)
} meta_t;

//container elements manipulations
dyndata_t* allocate_dyn(meta_t* meta, const void* elem);
int free_dyn(dyndata_t* elem);
int swap_dyn(dyndata_t* a, dyndata_t* b);
void* getdata(dyndata_t* cur);
dyndata_t* getright(dyndata_t* cur);
dyndata_t* getleft(dyndata_t* cur);

#endif // CSTRUCTURESCOMMON_H_INCLUDED
