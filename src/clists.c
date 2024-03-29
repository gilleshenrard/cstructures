/**
 * @file clists.c
 * @brief Implement bidirectional dynamic linked lists
 * @author Gilles Henard
 * @date 07/01/2024
 */
#include "clists.h"
#include <stdlib.h>
#include <string.h>


/*********************************************************************************************/
/*********************************************************************************************/


/**
 * @brief Sort the provided linked list (from smallest to biggest) using the Bubble Sort algorithm
 * 
 * @param meta      Meta data necessary to the algorithm
 * @param nbToSort  Number of biggest elements to sort at the end of the list
 * @retval  0       Sorted
 * @retval -1       Error
 */
int bubbleSortList(meta_t* meta, uint32_t nbToSort){
    dyndata_t *current=NULL, *next=NULL, *sentryRight = NULL;
    int swapped = 0;
    uint32_t remainingToSort = nbToSort;

    //no meta data available
    if(!meta || !meta->doCompare)
        return -1;

    //list is empty
    if(!meta->structure)
        return 0;

    do{
        swapped = 0;
        current = meta->structure;
        next = current->right;

        while(next && (next != sentryRight)){
            //if current element higher
            if((*meta->doCompare)(current->data, next->data) > 0)
            {
                //update the data structure address
                if(meta->structure == current)
                    meta->structure = next;

                //swap addresses of the elements
                if(swap_dyn(current, next) < 0)
                {
                    if(meta->doPError)
                        (*meta->doPError)("bubbleSortList: error while swapping elements");

                    return -1;
                }
                swapped = 1;
            }
            //get to the next element
            current = next;
            next = current->right;
        }
        //set the sentry to the current element
        sentryRight = current;
        remainingToSort--;
    }while(swapped && remainingToSort);

    return 0;
}

/**
 * @brief Get the nth element of a list
 * 
 * @param meta  Metadata necessary to the algorithm
 * @param i     Index of the element to get
 * @return Element sought after
 * @retval NULL Not found
 */
void* get_listelem(meta_t* meta, uint32_t i)
{
    dyndata_t *tmp = meta->structure, *next = NULL;
    uint32_t index = 0;

    if(i >= meta->nbelements)
    {
        if(meta->doPError)
            (*meta->doPError)("get_listelem: no element at index %d", i);

        return NULL;
    }

    //first element requested
    if(!i)
        return ((dyndata_t*)meta->structure)->data;

    //last element requested
    if(i == meta->elementsize - 1)
        return meta->last->data;

    //element between the first and the last
    next = tmp->right;
    while(index<i)
    {
        tmp = next;
        next = tmp->right;
        index++;
    }

    return tmp->data;
}

/**
 * @brief Search an element in a non-sorted list
 * 
 * @param meta      Metadata necessary to the algorithm
 * @param toSearch  Element to search
 * @return Adress of the element
 * @retval NULL Not found
 */
void* find_listelem(meta_t* meta, void* toSearch)
{
    dyndata_t *current = (dyndata_t *)meta->structure;

    while(current && (*meta->doCompare)(current->data, toSearch))
        current = current->right;

    return current;
}

/**
 * @brief Search an element in a sorted list
 * 
 * @param meta      Metadata necessary to the algorithm
 * @param toSearch  Element to search
 * @return          Address of the element
 * @retval NULL     Not found
 */
void* find_listelemSorted(meta_t* meta, void* toSearch)
{
    if(!meta || !meta->structure)
        return NULL;

    dyndata_t *current = (dyndata_t *)meta->structure;

    while((*meta->doCompare)(current->data, toSearch) < 0)
        current = current->right;

    if(current->data && ((*meta->doCompare)(current->data, toSearch) == 0))
        return current;
    else
        return NULL;
}

/**
 * @brief Insert an element at the top of a linked list
 * 
 * @param meta  Metadata necessary to the algorithm
 * @param toAdd Element to append in the list
 * @retval  0 Added
 * @retval -1 Error
 */
int insertListTop(meta_t* meta, void *toAdd){
    dyndata_t *newElement = NULL, *tmp=NULL;

    //check if meta data available
    if(!meta)
        return -1;

    //check if element added is available
    if(!toAdd)
    {
        if(meta->doPError)
            (*meta->doPError)("insertListTop: list metadata or mandatory function not defined");

        return -1;
    }

    if((newElement = allocate_dyn(meta, toAdd)) == NULL)
    {
        if(meta->doPError)
            (*meta->doPError)("insertListTop: new element could not be allocated");
        return -1;
    }

    //chain new element's previous pointer to list, if existing
    if(meta->structure){
        //chain new element's next pointer to list
        newElement->right = meta->structure;

        //chain list's head previous pointer to new element
        tmp = meta->structure;
        tmp->left = newElement;
    }
    else{
        meta->nbelements = 0;
        meta->last = newElement;
    }

    //make the new element head of the list
    meta->structure = newElement;

    //increment the elements counter
    meta->nbelements++;

    return 0;
}

/**
 * @brief Insert an element at the bottom of a linked list
 * 
 * @param meta  Metadata necessary to the algorithm
 * @param toAdd Element to append in the list
 * @retval  0 Added
 * @retval -1 Error
 */
int insertListBottom(meta_t* meta, void *toAdd){
    dyndata_t *newElement = NULL;

    //check if meta data available
    if(!meta)
        return -1;

    //check if element added is available
    if(!toAdd)
    {
        if(meta->doPError)
            (*meta->doPError)("insertListBottom: list metadata or mandatory function not defined");

        return -1;
    }

    if((newElement = allocate_dyn(meta, toAdd)) == NULL)
    {
        if(meta->doPError)
            (*meta->doPError)("insertListBottom: new element could not be allocated");
        return -1;
    }

    if(!meta->structure)
    {
        //list empty, element becomes the head
        meta->structure = newElement;
    }
    else
    {
        meta->last->right = newElement;

        //chain up the new element at the end
        newElement->left = meta->last;
    }

    //increment the elements counter
    meta->nbelements++;
    meta->last = newElement;

    return 0;
}

/**
 * @brief Remove the first element of the list
 * 
 * @param meta Metadata necessary to the algorithm
 * @retval  0 Popped
 * @retval -1 Error
 */
int popListTop(meta_t* meta){
    dyndata_t *head = NULL, *second=NULL;

    //check if meta data available
    if(!meta)
        return -1;

    //Structure is empty
    if(!meta->structure)
        return 0;

    //save list head and retrieve next element
    head = meta->structure;
    second = head->right;

    //free and rechain
    //  note : free() takes a void pointer anyway, so no need to cast
    free_dyn(&head);
    if(second && second->left)
        second->left = NULL;
    meta->structure = second;

    //update the number of elements
    meta->nbelements--;
    if(!meta->nbelements)
        meta->last = NULL;

    return 0;
}

/**
 * @brief Remove the last element of the list
 * 
 * @param meta Metadata necessary to the algorithm
 * @retval  0 Popped
 * @retval -1 Error
 */
int popListBottom(meta_t* meta){
    dyndata_t *tail = NULL, *second=NULL;

    //check if meta data available
    if(!meta)
        return -1;

    //Structure is empty
    if(!meta->structure)
        return 0;

    //save list tail and retrieve next element
    tail = meta->last;
    second = tail->left;

    //free and rechain
    //  note : free() takes a void pointer anyway, so no need to cast
    free_dyn(&tail);
    if(second && second->right)
        second->right = NULL;
    meta->last = second;

    //update the number of elements
    meta->nbelements--;
    if(!meta->nbelements)
        meta->last = NULL;

    return 0;
}

/**
 * @brief Insert an element at the right place in a sorted linked list
 * 
 * @param meta  Metadata necessary to the algorithm
 * @param toAdd Element to insert in the list
 * @return Element added
 * @retval NULL Error 
 */
int insertListSorted(meta_t *meta, void* toAdd){
    dyndata_t *newElement = NULL, *previous=NULL, *current=meta->structure;

    //non-existing list or element is supposed to become the first element
    if(!meta->structure || (*meta->doCompare)(toAdd, current->data) <= 0)
        return insertListTop(meta, toAdd);

    //element value is higher than last element, should then be last
    if((*meta->doCompare)(toAdd, meta->last->data) >= 0)
        return insertListBottom(meta, toAdd);

    if((newElement = allocate_dyn(meta, toAdd)) == NULL)
    {
        if(meta->doPError)
            (*meta->doPError)("insertListSorted: new element could not be allocated");

        return -1;
    }

    //walk through the list until the right place is found
    while(current!=NULL && (*meta->doCompare)(newElement->data,current->data)>0){
        previous = current;
        current = current->right;
    }

    //chain new element
    if(previous)
        previous->right = newElement;
    newElement->right = current;
    if(!current)
        meta->last = newElement;

    //update the element count
    meta->nbelements++;

    return 0;
}

/**
 * @brief Remove an element from a sorted linked list
 * 
 * @param meta Metadata necessary to the algorithm
 * @param elem Element to remove from the list
 * @retval  0 Item removed
 * @retval -1 Error
 */
int removeListSorted(meta_t *meta, void *elem){
    dyndata_t *previous = NULL, *current = NULL, *next = NULL;

    //metadata not provided
    if(!meta || !meta->doCompare)
        return -1;

    //list is empty
    if(!meta->structure)
        return 0;

    //search for the first occurrence of the element to remove from the list
    current = meta->structure;
    next = current->right;
    while((*meta->doCompare)(current->data, elem) < 0 && next){
        previous = current;
        current = next;
        next = next->right;
    }

    //if element found, get it out of the list and free its memory
    if(!(*meta->doCompare)(current->data, elem)){
        if(!previous)
            return popListTop(meta);
        if(!next)
            return popListBottom(meta);

        previous->right = next;
        next->left = previous;
        free_dyn(&current);

        //decrement the amount of elements
        meta->nbelements--;
        if(!meta->nbelements)
            meta->last = NULL;
    }

    return 0;
}

/**
 * @brief Free the memory of a list and its data
 * 
 * @param meta Metadata necessary to the algorithm
 * @retval  0 OK
 * @retval -1 Error
 */
int freeDynList(meta_t* meta)
{
    dyndata_t *next = NULL, *current=NULL;

    if(!meta)
        return -1;

    next = meta->structure;

    while(next)
    {
        current = next;
        next = next->right;
        free_dyn(&current);
    }

    meta->structure = NULL;
    meta->last = NULL;

    return 0;
}

/**
 * @brief Perform an action on every element of the list
 * 
 * @param meta      Metadata necessary to the algorithm
 * @param parameter Parameter for the action to perform
 * @param doAction  Action to perform
 * @retval  0 OK
 * @retval -1 Error
 */
int foreachList(meta_t* meta, void* parameter, int (*doAction)(void*, void*)){
    dyndata_t *next = NULL, *current=NULL;

    if(!meta)
        return -1;

    if(!doAction)
    {
        if(meta->doPError)
            (*meta->doPError)("foreachList: action to perform not defined");

        return -1;
    }

    next = meta->structure;

    while(next){
        current = next;
        next = next->right;
        if((*doAction)(current->data, parameter) < 0)
        {
            if(meta->doPError)
                (*meta->doPError)("foreachList: action specified returned with an error");

            return -1;
        }
    }

    return 0;
}
