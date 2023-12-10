/********************************************************
 * @file carrays.c
 * @brief Implements generic arrays
 * @author Gilles Henrard
 * @date 2023-12-10
********************************************************/
#include "carrays.h"
#include <stdlib.h>
#include <string.h>

//static functions
static uint32_t quickSortPartitioning(meta_t*, uint32_t, uint32_t);


/*********************************************************************************************/
/*********************************************************************************************/


/********************************************************
 * @brief Get an element of the array
 * 
 * @param meta  Metadata used by the array
 * @param i     Index of the element to get
 * @return      Array element
 * @retval NULL Element not found
********************************************************/
void* get_arrayelem(meta_t* meta, uint32_t i)
{
    if(i >= meta->nbelements)
    {
        if(meta->doPError)
            (*meta->doPError)("get_arrayelem: no element at the index %" PRIu64 "\n", i);

        return NULL;
    }
    else
        return ((uint8_t*)meta->structure) + (meta->elementsize * i);
}

/********************************************************
 * @brief Assign a value to an element of the array
 * 
 * @param meta  Metadata used by the array
 * @param i     Index of the element
 * @param elem  Element to assign
 * @retval 0    OK
 * @retval -1   Error
********************************************************/
int set_arrayelem(meta_t* meta, uint32_t i, void* elem)
{
    if(i >= meta->nbelements)
    {
        if(meta->doPError)
            (*meta->doPError)("set_arrayelem: index %" PRIu64 " out of range\n", i);

        return -1;
    }

    memcpy(((uint8_t*)meta->structure) + (meta->elementsize * i), elem, meta->elementsize);
    return 0;
}

/********************************************************
 * @brief Free the memory occupied by the whole array
 * 
 * @param meta  Metadata used by the array
 * @return 0
********************************************************/
int empty_array(meta_t* meta){
    //free the array itself
    free(meta->structure);
    meta->structure = NULL;

    //reset rest of the data
    meta->last = NULL;
    meta->nbelements = 0;

    return 0;
}

/********************************************************
 * @brief Sorts the array using the Bubble Sort algorithm
 * 
 * @param meta  Metadata used by the array
 * @param nb    Number of elements to sort at the end of the array
 * @retval 0    OK
 * @retval -1   Error
********************************************************/
int bubbleSortArray(meta_t *meta, uint32_t nb){
    void *current=NULL, *next=NULL;
    void* tmp = NULL;

    //no meta data available
    if(!meta || !meta->doCompare)
        return -1;

    //array is empty
    if(!meta->structure)
        return 0;

    //allocate the size of a temporary element in order to allow swapping
    tmp = calloc(1, meta->elementsize);
    if(!tmp)
    {
        if(meta->doPError)
            (*meta->doPError)("bubbleSortArray: temporary buffer could not be allocated");

        return -1;
    }

    for(uint32_t i=0 ; i<nb ; i++){
        for(uint32_t j=0 ; j<meta->nbelements-i-1 ; j++){
            //properly place the cursors
            current = get_arrayelem(meta, j);
            next = get_arrayelem(meta, j+1);

            if((*meta->doCompare)(current, next) > 0)
            {
                //swap the elements
                memcpy(tmp, current, meta->elementsize);
                memcpy(current, next, meta->elementsize);
                memcpy(next, tmp, meta->elementsize);
            }
        }
    }

    free(tmp);
    return 0;
}

/********************************************************
 * @brief Sort the partitions provided by the Quick Sort algorithm
 * 
 * @param meta  Metadata used by the array
 * @param low   Lowest element of the partition
 * @param high  Highest element of the partition
 * @return      New pivot
********************************************************/
static uint32_t quickSortPartitioning(meta_t* meta, uint32_t low, uint32_t high){
    void* pivotElem = NULL, *elem_i=NULL, *elem_j=NULL, *tmp = NULL;
    uint32_t i = 0;

    //get the element at the highest index of the partition for the pivot
    //      and place i below the lowest
    pivotElem = get_arrayelem(meta, high);
    i = low - 1;

    //allocate the size of a temporary element in order to allow swapping
    tmp = calloc(1, meta->elementsize);
    if(!tmp)
    {
        if(meta->doPError)
            (*meta->doPError)("quickSortPartitioning: temporary buffer could not be allocated");

        return -1;
    }

    //swap the elements until the pivot is at the right place
    //      with lower elements before, and higher ones after
    for(uint32_t j=low ; j<=high-1 ; j++){
        elem_j = get_arrayelem(meta, j);
        if((*meta->doCompare)(elem_j, pivotElem) < 0){
            i++;
            elem_i = get_arrayelem(meta, i);
            memcpy(tmp, elem_i, meta->elementsize);
            memcpy(elem_i, elem_j, meta->elementsize);
            memcpy(elem_j, tmp, meta->elementsize);
        }
    }

    //place the pivot at the right place by swapping i+1 and high
    //      (uses elem_i and elem_j for the sake of not creating new pointers)
    elem_i = get_arrayelem(meta, i+1);
    elem_j = get_arrayelem(meta, high);
    memcpy(tmp, elem_i, meta->elementsize);
    memcpy(elem_i, elem_j, meta->elementsize);
    memcpy(elem_j, tmp, meta->elementsize);

    free(tmp);
    return(i+1);
}

/********************************************************
 * @brief Sort the provided array using the Quick Sort algorithm
 * 
 * @param meta  Metadata used by the array
 * @param low   Lowest index in the array (most likely 0)
 * @param high  Highest index in the array (last element)
 * @retval 0    OK
 * @retval -1   Error
********************************************************/
int quickSortArray(meta_t* meta, uint32_t low, uint32_t high){
    uint32_t diffLow, diffHigh;

    //no meta data available
    if(!meta || !meta->doCompare)
        return -1;

    //list is empty
    if(!meta->structure)
        return 0;

    //as low and high are unsigned, calculate their distance to meta->nbelements
    diffLow = (low > meta->nbelements ? (UINT32_MAX - low) + meta->nbelements : meta->nbelements - low);
    diffHigh = (high > meta->nbelements ? (UINT32_MAX - high) + meta->nbelements : meta->nbelements - high);

    //if current partition not yet entirely sorted
    if(diffLow > diffHigh){
        uint32_t pivot;

        //place the pivot at its right place
        pivot = quickSortPartitioning(meta, low, high);

        //sort the partition below pivot
        if(quickSortArray(meta, low, pivot-1) < 0)
        {
            if(meta->doPError)
                (*meta->doPError)("quickSortArray: error while sorting the array below index %d", pivot-1);

            return -1;
        }

        //sort the partition above pivot
        if(quickSortArray(meta, pivot+1, high) < 0)
        {
            if(meta->doPError)
                (*meta->doPError)("quickSortArray: error while sorting the array above index %d", pivot+1);

            return -1;
        }
    }

    return 0;
}

/********************************************************
 * @brief Search an element using the Binary Search algorithm
 * 
 * @param meta      Metadata used by the array
 * @param toSearch  Element to search
 * @param scope     Scope of the search (first occurence or any occurrence)
 * @return          Index of the occurence found in the array
 * @retval -1       Not found
********************************************************/
int binarySearchArray(meta_t *meta, void* toSearch, e_search scope){
    int i = 0, j = (meta->nbelements - 1), index = -1;
    void *current = NULL;

    while(i <= j)
    {
        int m = (i + j) / 2;
        //position the cursor
        current = get_arrayelem(meta, m);

        if((*meta->doCompare)(current, toSearch) < 0)
            i = m + 1;
        else
        {
            if((*meta->doCompare)(current, toSearch) > 0)
                j = m - 1;
            else
            {
                index = m;
                if(scope == FIRST)
                    j = m - 1;
                else
                    i = j + 1;
            }
        }
    }

    return index;
}

/********************************************************
 * @brief Perform an action on each element of the array
 * 
 * @param meta      Metadata used by the array
 * @param parameter Parameter for the action to perform
 * @param doAction  Action to perform
 * @retval 0        OK
 * @retval -1       Error
********************************************************/
int foreachArray(meta_t* meta, void* parameter, int (*doAction)(void*, void*)){
    void* tmp = NULL;

    for(uint32_t i=0 ; i<meta->nbelements ; i++){
        //position the pointer properly
        tmp = get_arrayelem(meta, i);
        //execute action
        if((*doAction)(tmp, parameter) < 0)
        {
            if(meta->doPError)
                (*meta->doPError)("foreachArray: action specified returned with an error");

            return -1;
        }
    }

    return 0;
}
