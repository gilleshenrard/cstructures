#include "dataset_test.h"

/****************************************************************************************/
/*  I : dataset record to print                                                         */
/*  P : Prints an dataset record                                                        */
/*  O : /                                                                               */
/****************************************************************************************/
#ifdef __GNUC__
# pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
int Print_dataset(void* elem, void* nullable)
{
    dataset_t* A=(dataset_t*)elem;

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
    dataset_t* A = (dataset_t*)elem;

    return A->type;
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
int compare_dataset(void* a, void* b)
{
    dataset_t* A=(dataset_t*)a, *B=(dataset_t*)b;

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
int compare_dataset_int(void* a, void* b)
{
    dataset_t* A = (dataset_t*)a;
    int* B = (int*)b;

    if(A->id > *B)
        return 1;
    else if(A->id < *B)
        return -1;
    else
        return 0;
}
