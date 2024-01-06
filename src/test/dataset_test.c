/**
 * @file dataset_test.c
 * @author Gilles Henrard
 * @brief Dummy structure used to test out the algorithms
 * @date 06/01/2024
 */
#include "dataset_test.h"
#include <string.h>
#include <stdio.h>

/**
 * @brief Prints a dataset record on screen
 * 
 * @param elem      Element to print
 * @param nullable  Unused parameter
 * @retval 0        Success
 */
int Print_dataset(void* elem, void* nullable)
{
    const dataset_t* A=(dataset_t*)elem;
    (void)nullable;

    printf("%2d,%32s%4.8f\n", A->id, A->type, A->price);
    return 0;
}

/**
 * @brief Returns a string representing the dataset
 * @note This just returns the type field value of the record
 * 
 * @param elem      Record to print
 * @return char*    Record type
 */
char* toString_dataset(void* elem)
{
    const dataset_t* A = (dataset_t*)elem;

    return (char*)A->type;
}

/**
 * @brief Compares two datasets by their ID and name
 * 
 * @param a First dataset to compare
 * @param b Second dataset to compare
 * @retval  1 A > B
 * @retval  0 A = B
 * @retval -1 A < B
 */
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

/**
 * @brief Compares two datasets by their ID
 * 
 * @param a First dataset to compare
 * @param b Second dataset to compare
 * @retval  1 A > B
 * @retval  0 A = B
 * @retval -1 A < B
 */
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
