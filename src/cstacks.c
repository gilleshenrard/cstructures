/**
 * @file cstacks.c
 * @brief Implement bidirectional dynamic stacks
 * @author Gilles Henrard
 * @date 18/12/2023
 */
#include "cstacks.h"
#include <stdlib.h>

/**
 * @brief Push a new element at the head of the stack
 * 
 * @param meta  Metadata used by the stack
 * @param toAdd Element to add in the stack
 * @retval  0 Element added
 * @retval -1 Error
 */
int pushStack(meta_t *meta,  const void *toAdd){
    dyndata_t *newElement = NULL;

    //check if meta data available
    if(!meta)
        return -1;

    //check if element added is available
    if(!toAdd)
    {
        if(meta->doPError)
            (*meta->doPError)("pushStack: new element cannot be NULL");

        return -1;
    }

    //allocate memory for the new element
    if((newElement = allocate_dyn(meta, toAdd)) == NULL)
    {
        if(meta->doPError)
            (*meta->doPError)("pushStack: new element could not be allocated");
        return -1;
    }

    //if structure already exists
    if(meta->structure){
        //chain the element to the head of the stack
        newElement->right = meta->structure;
        ((dyndata_t *)meta->structure)->left = newElement;
        meta->structure = newElement;

        //increment the amount of elements
        meta->nbelements++;
    }
    else{
        //otherwise set all pointers and nbelements counter
        meta->nbelements = 1;
        meta->last = newElement;
        meta->structure = newElement;
    }

    return 0;
}

/**
 * @brief Pop the head from the stack
 * 
 * @param meta Metadata used by the stack
 * @return Head of the stack
 * @retval NULL Error or queue empty
 */
void* popStack(meta_t* meta){
    dyndata_t *tmp = NULL;

    //check if meta data available
    if(!meta)
        return NULL;

    //get the first element of the stack
    tmp = meta->structure;

    //if not empty
    if(tmp)
    {
        //update the head of the stack + update elements counter
        meta->structure = tmp->right;
        meta->nbelements--;

        //if stack now empty, update pointers
        if(!meta->nbelements){
            meta->structure = NULL;
            meta->last = NULL;
        }
    }

    return tmp;
}

/**
 * @brief Perform an action on every element of the stack
 * 
 * @param meta      Metadata used by the stack
 * @param parameter Parameter used by the action to perform
 * @param doAction  Action to perform
 * @retval  0 Element added
 * @retval -1 Error
 */
int foreachStack(meta_t* meta, void* parameter, int (*doAction)(void*, void*)){
    dyndata_t *next = NULL, *current=NULL;

    if(!meta)
        return -1;

    if(!doAction)
    {
        if(meta->doPError)
            (*meta->doPError)("foreachStack: action to perform not defined");

        return -1;
    }

    next = meta->structure;

    while(next){
        current = next;
        next = next->right;
        if((*doAction)(current->data, parameter) < 0)
        {
            if(meta->doPError)
                (*meta->doPError)("foreachStack: action specified returned with an error");

            return -1;
        }
    }

    return 0;
}
