/*
** cstructurescommon.c
** Library defining the key elements to handle the cstructures library
** ---------------------------------------------------
** Made by Gilles Henrard
** Last modified : 21/07/2022
*/
#include "cstructurescommon.h"
#include <stdlib.h>
#include <string.h>

/************************************************************/
/*  I : Metadata of the element to allocate                 */
/*      Actual data of the element to allocate              */
/*  P : Allocates memory for a dynamic element and copy its */
/*          value                                           */
/*  O : Address of the element if OK                        */
/*      NULL if error                                       */
/************************************************************/
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

/************************************************************/
/*  I : Element to deallocate                               */
/*  P : Deallocates the memory for a dynamic element        */
/*  O : /                                                   */
/************************************************************/
int free_dyn(dyndata_t* elem)
{
    free(elem->data);
    free(elem);
    elem = NULL;

    return 0;
}

/************************************************************/
/*  I : Elements to swap                                    */
/*  P : Swaps the pointers of the elements                  */
/*  O : /                                                   */
/************************************************************/
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

/************************************************************/
/*  I : Element of which get the data                       */
/*  P : Returns the data of the element                     */
/*  O : NULL -> element not allocated                       */
/*      Otherwise -> address of the data element            */
/************************************************************/
void* getdata(dyndata_t* cur)
{
    if(cur)
        return cur->data;
    else
        return NULL;
}

/************************************************************/
/*  I : Element of which get the next one in the structure  */
/*  P : Returns the address of the next element             */
/*  O : NULL -> no element to the right                     */
/*      Otherwise -> address of the next element            */
/************************************************************/
dyndata_t* getright(dyndata_t* cur)
{
    if(cur)
        return cur->right;
    else
        return NULL;
}
