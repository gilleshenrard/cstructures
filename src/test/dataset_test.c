#include "dataset_test.h"
#include <string.h>
#include <stdio.h>

/****************************************************************************************/
/*  I : dataset record to print                                                         */
/*  P : Prints an dataset record                                                        */
/*  O : /                                                                               */
/****************************************************************************************/
int Print_dataset(void* elem, void* nullable)
{
    const dataset_t* A=(dataset_t*)elem;
    (void)nullable;

    printf("%2d,%32s%4.8f\n", A->id, A->type, A->price);
    return 0;
}

/************************************************************/
/*  I : record to summarise as a string                     */
/*      /                                                   */
/*  P : returns a string representing the dataset           */
/*  O : /                                                   */
/************************************************************/
char* toString_dataset(void* elem)
{
    const dataset_t* A = (dataset_t*)elem;

    return (char*)A->type;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// DYNAMIC ALLOCATION METHODS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/****************************************************************************************/
/*  I : first dataset to compare                                                        */
/*      second dataset to compare                                                       */
/*  P : Compares two datasets by their ID and name                                      */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_dataset(const void* a, const void* b)
{
    const dataset_t* A=(dataset_t*)a, *B=(dataset_t*)b;

    if(A->id > B->id)
        return 1;
    else if(A->id < B->id)
        return -1;
    else
        return strcmp(A->type, B->type);
}

/****************************************************************************************/
/*  I : first dataset to compare                                                        */
/*      second dataset to compare                                                       */
/*  P : Compares two datasets by their ID                                               */
/*  O :  1 if A > B                                                                     */
/*       0 if A = B                                                                     */
/*      -1 if A < B                                                                     */
/****************************************************************************************/
int compare_dataset_int(const void* a, const void* b)
{
    const dataset_t* A = (dataset_t*)a;
    int* B = (int*)b;

    if(A->id > *B)
        return 1;
    else if(A->id < *B)
        return -1;
    else
        return 0;
}
