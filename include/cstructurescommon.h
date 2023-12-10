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

/********************************************************
 * @brief Forward-declaration of dyndata_t structures
 * @note Mandatory to use a reference of a structure in itself
********************************************************/
typedef struct dyndata_t dyndata_t;

/********************************************************
 * @brief Structure defining a dynamic element used in all data structures
********************************************************/
struct dyndata_t
{
    void*       data;   ///< Address of the memory used by the element
    int         height; ///< Height of the element in the structure @note Used in AVL trees
    dyndata_t*  left;   ///< Address of the element to the left of the current one
    dyndata_t*  right;  ///< Address of the element to the right of the current one
};

/********************************************************
 * @brief Structure defining the metadata of a data structure
********************************************************/
typedef struct
{
    void*       structure;                              ///< address of the root of the structure
    dyndata_t*  last;                                   ///< highest element in the structure
    uint32_t    nbelements;                             ///< number of elements in the structure
    uint32_t    elementsize;                            ///< size of a core element (actual data without algorithmic overlay)
    int         (*doCompare)(const void*, const void*); ///< comparison method
    void        (*doPError)(char* msg, ...);            ///< error printing method (can be ignored if set as NULL)
} meta_t;

//container elements manipulations
void initialise_structure(meta_t* meta, const uint32_t elementSize, int (*compare)(const void*, const void*), void (*printError)(char* msg, ...));
dyndata_t* allocate_dyn(meta_t* meta, const void* elem);
int free_dyn(dyndata_t** elem);
int swap_dyn(dyndata_t* a, dyndata_t* b);
void* getdata(dyndata_t* cur);
dyndata_t* getright(dyndata_t* cur);
dyndata_t* getleft(dyndata_t* cur);
dyndata_t* getFirst(const meta_t* meta);
dyndata_t* getLast(const meta_t* meta);

#endif // CSTRUCTURESCOMMON_H_INCLUDED
