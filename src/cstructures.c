/*
** cstructures.c
** Library regrouping algorithmic-based functions
** Structures available :
**   - Bidirectional lists
**   - Arrays
**   - AVL trees
** ---------------------------------------------------
** Made by Gilles Henrard
** Last modified : 21/07/2022
*/
#include "cstructures.h"
#include <stdlib.h>
#include <string.h>

/************************************************************/
/*  I : List to copy                                        */
/*      Array to create (MUST BE EMPTY)                     */
/*      Action to perform on the list members (free or not) */
/*  P : Allocates memory for the array and copies the list  */
/*          into it                                         */
/*  O :  0 -> Array created                                 */
/*      -1 -> Error                                         */
/************************************************************/
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

/************************************************************/
/*  I : Array to copy                                       */
/*      List to create                                      */
/*      Action to perform on the list members (free or not) */
/*  P : Allocates memory for the list and copies the array  */
/*          into it                                         */
/*  O :  0 -> List created                                  */
/*      -1 -> Error                                         */
/************************************************************/
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

/************************************************************/
/*  I : Array to copy                                       */
/*      AVL to create                                       */
/*      Action to perform on the list members (free or not) */
/*  P : Allocates memory for the AVL and copies the array   */
/*          into it                                         */
/*  O :  0 -> AVL created                                   */
/*      -1 -> Error                                         */
/************************************************************/
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
