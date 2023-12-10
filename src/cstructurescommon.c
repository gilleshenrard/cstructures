/**
 * @file cstructurescommon.c
 * @brief Implements the key elements to handle the cstructures library
 * @author Gilles Henrard
 * @date 2023-12-10
 * 
 * @warning
 * - About data structures:
*       when dealing with arrays, <meta_t>.structure can direcly be set with a previously generated array.
*       when dealing with AVL trees or dynamic lists, dyndata_t elements will be automatically generated,
*       and are not to be handled by the user
*
* - About error printing:
*       if doPError is set as NULL, error printing will be ignored
*/
#include "cstructurescommon.h"
#include <stdlib.h>
#include <string.h>


/*********************************************************************************************/
/*********************************************************************************************/


/**
 * @brief Initialise a structure
 * 
 * @param meta          Structure to initialise
 * @param elementSize   Size of an element in the structure
 * @param compare       Address of the function used to compare two elements
 * @param printError    Address of the function used to print errors
*/
void initialise_structure(meta_t* meta, const uint32_t elementSize, int (*compare)(const void*, const void*), void (*printError)(char* msg, ...))
{
    memset(meta, 0, sizeof(meta_t));
    meta->elementsize = elementSize;
    meta->doCompare = compare;
    meta->doPError = printError;
}

/**
 * @brief Allocate a structure element
 * 
 * @param meta  Metadata used by the current structure
 * @param elem  Data to copy in the new element
 * @return      Address of the new element
 * @retval NULL Error
*/
dyndata_t* allocate_dyn(meta_t* meta, const void* elem)
{
    dyndata_t* tmp=NULL;

    //memory allocation for the dynamic element
    tmp = calloc(1, sizeof(dyndata_t));
    if(!tmp)
    {
        if(meta->doPError)
            (*meta->doPError)("allocate_dyn: element could not be allocated");

        return NULL;
    }

    //memory allocation for the actual data
    tmp->data = calloc(1, meta->elementsize);
    if(!tmp->data)
    {
        if(meta->doPError)
            (*meta->doPError)("allocate_dyn: element data could not be allocated");

        free(tmp);
        return NULL;
    }

    //copy new element data and set AVL leaf height
    memcpy(tmp->data, elem, meta->elementsize);
    tmp->height = 1;

    return tmp;
}

/**
 * @brief Free the memory used by a dynamic element
 * 
 * @param elem  Element to free
 * @return      Success
*/
int free_dyn(dyndata_t** elem)
{
    free((*elem)->data);
    free(*elem);
    *elem = NULL;

    return 0;
}

/**
 * @brief Swap two elements in a structure
 * @note This is achieved by swapping their addresses
 * 
 * @param a First element to swap
 * @param b Second element to swap
 * @return  Success
*/
int swap_dyn(dyndata_t* a, dyndata_t* b)
{
    dyndata_t *prev=a->left, *next=b->right;

    a->right = b->right;
    b->left = a->left;
    a->left = b;
    b->right = a;

    if(prev)
        prev->right = b;
    if(next)
        next->left = a;

    return 0;
}

/**
 * @brief Get the data encased in a dynamic element
 * 
 * @param cur   Element of which get the data
 * @return      Address of the data in the element
 * @retval NULL Element not allocated
*/
void* getdata(dyndata_t* cur)
{
    if(cur)
        return cur->data;
    else
        return NULL;
}

/**
 * @brief Get the element at the right of the current one
 * 
 * @param cur   Element of which get the right element
 * @return      Element at the right
 * @retval NULL Element not allocated
*/
dyndata_t* getright(dyndata_t* cur)
{
    if(cur)
        return cur->right;
    else
        return NULL;
}

/**
 * @brief Get the element at the left of the current one
 * 
 * @param cur   Element of which get the left element
 * @return      Element at the left
 * @retval NULL Element not allocated
*/
dyndata_t* getleft(dyndata_t* cur)
{
    if(cur)
        return cur->left;
    else
        return NULL;
}

/**
 * @brief Get the root element of a data structure
 * 
 * @param meta  Metadata used by the structure
 * @return      Root element
*/
dyndata_t* getFirst(const meta_t* meta){
    if(meta)
        return (dyndata_t*)meta->structure;
    else
        return NULL;
}

/**
 * @brief Get the last element of a data structure
 * 
 * @param meta  Metadata used by the structure
 * @return      Last element
*/
dyndata_t* getLast(const meta_t* meta){
    if(meta)
        return (dyndata_t*)meta->last;
    else
        return NULL;
}
