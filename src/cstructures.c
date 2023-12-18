/**
 * @file cstructures.c
 * @brief Implement a structure-wise algorithms library
 * @author Gilles Henrard
 * @date 18/12/2023
 */
#include "cstructures.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Allocates memory an array and copies a list into it
 * 
 * @param dList     List to copy
 * @param dArray    Array to create @warning MUST BE EMPTY
 * @param action    Action to perform on the list members @note free or not
 * @retval  0 Array created
 * @retval -1 Error
 */
int listToArray(meta_t* dList, meta_t* dArray, e_listtoarray action){
    void *tmp_array = NULL;
    dyndata_t* tmp_list = NULL;

    //check if the array doesn't exist
    if(dArray->structure)
    {
        if(dArray->doPError)
            (*dArray->doPError)("listToArray: array is not NULL");

        return -1;
    }

    //allocate the memory
    dArray->structure = calloc(dList->nbelements, dList->elementsize);
    if(!dArray->structure)
    {
        if(dArray->doPError)
            (*dArray->doPError)("listToArray: array could not be allocated");

        return -1;
    }

    dArray->nbelements = dList->nbelements;

    //copy elements one by one in the array
    tmp_list = dList->structure;
    for(uint32_t i=0 ; i<dArray->nbelements ; i++){
        //position the pointer properly
        tmp_array = get_arrayelem(dArray, i);
        memcpy(tmp_array, tmp_list->data, dList->elementsize);

        //if desired, free the freshly copied element
        if(action == REPLACE){
            if(popListTop(dList) <0)
            {
                if(dList->doPError)
                    (*dList->doPError)("listToArray: error while deleting list's head");

                return -1;
            }
            tmp_list = dList->structure;
        }
        else{
            //increment the list pointer
            tmp_list = tmp_list->right;
        }
    }

    return 0;
}

/**
 * @brief Allocates memory for a list and copies an array into it
 * 
 * @param dArray    Array to copy
 * @param dList     List to create
 * @param action    Action to perform on the list members @note free or not
 * @retval  0 List created
 * @retval -1 Error
 */
int arrayToList(meta_t* dArray, meta_t* dList, e_listtoarray action){
    //copy elements one by one in the list
    for(uint32_t i=0 ; i<dArray->nbelements ; i++)
    {
        //insert in the list
        if(insertListBottom(dList,  get_arrayelem(dArray, i)) < 0)
        {
            if(dList->doPError)
                (*dList->doPError)("arrayToList: error while inserting in the list");

            return -1;
        }
    }

    //if desired, free the freshly copied element
    if(action == REPLACE)
        empty_array(dArray);

    return 0;
}

/**
 * @brief Allocates memory for the AVL and copies the array into it
 * 
 * @param dArray    Array to copy
 * @param dAVL      AVL to create
 * @param action    Action to perform on the list members @note free or not
 * @retval  0 AVL created
 * @retval -1 Error
 */
int arrayToAVL(meta_t* dArray, meta_t* dAVL, e_listtoarray action){
    dyndata_t* tmp_array = dArray->structure;

    //copy elements one by one in the list
    for(uint32_t i=0 ; i<dArray->nbelements ; i++){
        tmp_array = get_arrayelem(dArray, i);
        //insert in the AVL
        dAVL->structure = insertAVL(dAVL, dAVL->structure, tmp_array);
    }

    //if desired, free the freshly copied element
    if(action == REPLACE)
        empty_array(dArray);

    return 0;
}
