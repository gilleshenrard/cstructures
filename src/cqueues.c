/**
 * @file cqueues.c
 * @brief Implement dynamic queues with priority mechanisms
 * @author Gilles Henrard
 * @date 18/12/2023
 */
#include "cqueues.h"
#include <stdlib.h>

/**
 * @brief Push a new element at the back of the queue
 * 
 * @param meta  Metadata used by the queue
 * @param toAdd Element to push in the queue
 * @retval  0 Element pushed
 * @retval -1 Error
 */
int pushQueue(meta_t *meta,  const void *toAdd){
    dyndata_t *newElement = NULL;

    //check if meta data available
    if(!meta)
        return -1;

    //check if element added is available
    if(!toAdd)
    {
        if(meta->doPError)
            (*meta->doPError)("pushQueue: new element cannot be NULL");

        return -1;
    }

    //allocate memory for the new element
    if((newElement = allocate_dyn(meta, toAdd)) == NULL)
    {
        if(meta->doPError)
            (*meta->doPError)("pushQueue: new element could not be allocated");
        return -1;
    }

    //if structure already exists
    if(meta->structure){
        //chain the element to the back of the queue
        newElement->left = meta->last;
        meta->last->right = newElement;
        meta->last = newElement;

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
 * @brief Push a new element at the back of the priority queue
 * 
 * @param meta  Metadata used by the queue
 * @param toAdd Element to push in the queue
 * @retval  0 Element pushed
 * @retval -1 Error
 */
int pushPriorityQueue(meta_t *meta,  const void *toAdd){
    dyndata_t *newElement = NULL, *tmp = NULL, *tmp2=NULL;

    //check if meta data available
    if(!meta)
        return -1;

    //check if element added is available
    if(!toAdd)
    {
        if(meta->doPError)
            (*meta->doPError)("pushPriorityQueue: new element cannot be NULL");

        return -1;
    }

    //allocate memory for the new element
    if((newElement = allocate_dyn(meta, toAdd)) == NULL)
    {
        if(meta->doPError)
            (*meta->doPError)("pushPriorityQueue: new element could not be allocated");
        return -1;
    }

    //if no element
    if(!meta->structure){
        meta->nbelements = 1;
        meta->last = newElement;
        meta->structure = newElement;

        return 0;
    }

    tmp = (dyndata_t*)meta->structure;

    //if element has highest priority
    if((*meta->doCompare)(newElement->data, tmp->data) > 0){
        newElement->right = meta->structure;
        tmp->left = newElement;
        meta->structure = newElement;
    }
    else{
        //browse queue from last element until higher priority
        tmp = meta->last;
        while((*meta->doCompare)(newElement->data, tmp->data) > 0){
            tmp2 = tmp;
            tmp = tmp->left;
        }

        //chain the element to its place in the queue, with regard to priority
        newElement->right = tmp2;
        newElement->left = tmp;
        tmp->right = newElement;
        if(tmp2)
            tmp2->left = newElement;
        if(tmp == meta->last)
            meta->last = newElement;
    }

    //increment the amount of elements
    meta->nbelements++;

    return 0;
}

/**
 * @brief Pop the first element from the queue
 * 
 * @param meta Metadata used by the queue
 * @return Head of the queue
 * @retval NULL Error or queue empty
 */
void* popQueue(meta_t* meta){
    dyndata_t *tmp = NULL;

    //check if meta data available
    if(!meta)
        return NULL;

    //get the first element of the queue
    tmp = meta->structure;

    //if not empty
    if(tmp)
    {
        //update the head of the queue + update elements counter
        meta->structure = tmp->right;
        meta->nbelements--;

        //if queue now empty, update pointers
        if(!meta->nbelements){
            meta->structure = NULL;
            meta->last = NULL;
        }
    }

    return tmp;
}

/**
 * @brief Perform an action on every element of the queue
 * 
 * @param meta      Metadata used by the queue
 * @param parameter Parameter used by the action to perform
 * @param doAction  Action to perform
 * @retval  0 Action performed
 * @retval -1 Error
 */
int foreachQueue(meta_t* meta, void* parameter, int (*doAction)(void*, void*)){
    dyndata_t *next = NULL, *current=NULL;

    if(!meta)
        return -1;

    if(!doAction)
    {
        if(meta->doPError)
            (*meta->doPError)("foreachQueue: action to perform not defined");

        return -1;
    }

    next = meta->structure;

    while(next){
        current = next;
        next = next->right;
        if((*doAction)(current->data, parameter) < 0)
        {
            if(meta->doPError)
                (*meta->doPError)("foreachQueue: action specified returned with an error");

            return -1;
        }
    }

    return 0;
}
