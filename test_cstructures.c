/*
** test_structuresc
** Contains all the tests concerning algorithmic features in libcstructures.so
** -------------------------------------------
** Made by Gilles Henrard
** Last modified : 04/08/2022
*/
#include <time.h>
#include <stdlib.h>
#include "cstructures.h"
#include "dataset_test.h"

int setup_data(dataset_t** data, uint32_t nb);
int tst_bubblesortarray(void);
int tst_quicksortarray(void);
int tst_binarysearcharray(void);
int tst_inserttoplist(void);
int tst_insertbottomlist(void);
int tst_insertlistsorted(void);
int tst_searchlistsorted(void);
int tst_removelistsorted(void);
int tst_bubblesortlist(void);
int tst_structuresconversion(void);
int tst_insertavl(void);
int tst_removeavl(void);
int tst_searchavl(void);
int tst_insertqueue(void);
int tst_insertstack(void);

#ifdef __GNUC__
# pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
int main(int argc, char *argv[])
{
    srand(time(NULL));

    tst_bubblesortarray();
    tst_quicksortarray();
    tst_binarysearcharray();
    tst_inserttoplist();
    tst_insertbottomlist();
    tst_insertlistsorted();
    tst_searchlistsorted();
    tst_removelistsorted();
    tst_bubblesortlist();
    tst_structuresconversion();
    tst_insertavl();
    tst_removeavl();
    tst_searchavl();
    tst_insertqueue();
    tst_insertstack();

	exit(EXIT_SUCCESS);
}

/************************************************************/
/*  I : Pointer to the array to allocate                    */
/*      Number of elements to allocate                      */
/*  P : Allocates an array and fills it with random data    */
/*  O :  0 -> Array created                                 */
/*      -1 -> Error                                         */
/************************************************************/
int setup_data(dataset_t** data, uint32_t nb)
{
    uint32_t r = 0, r2 = 0;

    //allocate a space of nb times the size of dataset_t
    *data = calloc(nb, sizeof(dataset_t));
    if(!*data)
        return -1;

    for(uint32_t i=0 ; i<nb ; i++)
    {
        //generate a radom number between 1 and nb (inclusive),
        //  and a second between 1 and 5
        r = rand() % (2*nb) + 1;
        r2 = rand() % 10 + 1;

        //fill in information relative to the random number
        (*data)[i].id = r;
        sprintf((*data)[i].type, "value : %05d", r*r2);
        (*data)[i].price = (float)r * 3.141593;
    }

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Tests out the bubble sort algo with arrays          */
/*  O :  0 -> Success                                       */
/*      -1 -> Error                                         */
/************************************************************/
int tst_bubblesortarray()
{
    meta_t arr = {NULL, NULL, 20, sizeof(dataset_t), compare_dataset, NULL};

    printf("/******************************************************************/\n");
    printf("/********************* tst_bubblesortarray ************************/\n");
    printf("/******************************************************************/\n");

    //generate 20 random datasets
    if(setup_data((dataset_t**)&arr.structure, 20) == -1)
    {
        fprintf(stderr, "bubbleSortArray : error while allocating the data\n");
        return -1;
    }
    //display the unsorted data
    foreachArray(&arr, NULL, Print_dataset);

    printf("----------------------------------------------------------\n");
    //sort it
    if(bubbleSortArray(&arr, 3) == -1)
    {
        fprintf(stderr, "bubbleSortArray : error while sorting the data\n");
        empty_array(&arr);
        return -1;
    }

    printf("Data with the %d highest elements sorted:\n", 3);
    //display all the datasets
    foreachArray(&arr, NULL, Print_dataset);

    printf("----------------------------------------------------------\n");
    //sort it
    if(bubbleSortArray(&arr, arr.nbelements) == -1)
    {
        fprintf(stderr, "bubbleSortArray : error while sorting the data\n");
        empty_array(&arr);
        return -1;
    }

    printf("Data fully sorted:\n");
    //display all the datasets
    foreachArray(&arr, NULL, Print_dataset);

    //free memory
    empty_array(&arr);
    return 0;
}

/********************************************************************/
/*  I : /                                                           */
/*  P : Tests out the quick sort algo with array of 10000 elements  */
/*  O :  0 -> Success                                               */
/*      -1 -> Error                                                 */
/********************************************************************/
int tst_quicksortarray()
{
    meta_t arr = {NULL, NULL, 100000, sizeof(dataset_t), compare_dataset, NULL};
    uint32_t i = 0;
    int cmp = 0;

    printf("/******************************************************************/\n");
    printf("/********************* tst_quicksortarray *************************/\n");
    printf("/******************************************************************/\n");

    //generate 100000 random datasets
    if(setup_data((dataset_t**)&arr.structure, arr.nbelements) == -1)
    {
        fprintf(stderr, "quickSortArray : error while allocating the data\n");
        return -1;
    }

    printf("100000 elements created\n");
    printf("----------------------------------------------------------\n");

    //sort it
    if(quickSortArray(&arr, 0, arr.nbelements-1) == -1)
    {
        fprintf(stderr, "quickSortArray : error while sorting the data\n");
        empty_array(&arr);
        return -1;
    }

    //check if each element is lower than the next one
    i = -1;
    do{
        i++;
        cmp = (*arr.doCompare)(get_arrayelem(&arr, i), get_arrayelem(&arr, i+1));
    }while(i < arr.nbelements - 2 && cmp <= 0);

    if(cmp <= 0 && i == arr.nbelements - 2){
        printf("All %u elements properly sorted\n", arr.nbelements);
        printf("Displaying the first 50 elements :\n");
        for(i = 0 ; i < 50 ; i++)
            Print_dataset(get_arrayelem(&arr, i), NULL);
    }
    else{
        fprintf(stderr, "Elements %u and %u not properly sorted in the array :\n", i, i+1);
        Print_dataset(get_arrayelem(&arr, i), NULL);
        Print_dataset(get_arrayelem(&arr, i+1), NULL);
        empty_array(&arr);
        return -1;
    }

    //free memory
    empty_array(&arr);
    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Tests out the binary search algo with arrays        */
/*  O :  0 -> Success                                       */
/*      -1 -> Error                                         */
/************************************************************/
int tst_binarysearcharray()
{
    meta_t arr = {NULL, NULL, 20, sizeof(dataset_t), compare_dataset, NULL};
    dataset_t* tmp = NULL;
    int found=0;

    printf("/*********************************************************************/\n");
    printf("/********************* tst_binarysearcharray *************************/\n");
    printf("/*********************************************************************/\n");

    //generate 20 random datasets
    if(setup_data((dataset_t**)&arr.structure, 20) == -1)
    {
        fprintf(stderr, "binarysearchArray : error while allocating the data\n");
        return -1;
    }

    //sort it
    if(quickSortArray(&arr, 0, arr.nbelements-1) == -1)
    {
        fprintf(stderr, "binarysearchArray : error while sorting the data\n");
        empty_array(&arr);
        return -1;
    }

    //display the sorted data
    foreachArray(&arr, NULL, Print_dataset);
    printf("----------------------------------------------------------\n");

    tmp = get_arrayelem(&arr, 3);
    arr.doCompare = compare_dataset_int;

    //search for element with ID same as at index 3
    found = binarySearchArray(&arr, &tmp->id, ANY);
    if(found == -1)
        printf("%d was not found\n", tmp->id);
    else
        printf("%d was found at index %d\n", tmp->id, found);

    //search for first occurence of elemnt with ID same as at index 3
    found = binarySearchArray(&arr, &tmp->id, FIRST);
    if(found == -1)
        printf("First occurence of %d was not found\n", tmp->id);
    else
        printf("First occurence of %d was found at index %d\n", tmp->id, found);

    //free memory
    empty_array(&arr);
    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Tests out the insertion at the head of a list       */
/*  O :  0 -> Success                                       */
/*      -1 -> Error                                         */
/************************************************************/
int tst_inserttoplist()
{
    meta_t arr = {NULL, NULL, 20, sizeof(dataset_t), compare_dataset, NULL};
    meta_t lis = {NULL, NULL, 0, sizeof(dataset_t), compare_dataset, NULL};

    printf("/*********************************************************************/\n");
    printf("/********************* tst_inserttoplist *****************************/\n");
    printf("/*********************************************************************/\n");

    //generate 20 random datasets
    if(setup_data((dataset_t**)&arr.structure, 20) == -1)
    {
        fprintf(stderr, "insertTopList : error while allocating the data\n");
        return -1;
    }

    //display the sorted data
    foreachArray(&arr, NULL, Print_dataset);
    printf("----------------------------------------------------------\n");

    for(int i=0 ; i<20 ; i++)
    {
        if(insertListTop(&lis, arr.structure + i*sizeof(dataset_t)) == -1)
        {
            fprintf(stderr, "insertTopList : error while inserting the data\n");
            empty_array(&arr);
            freeDynList(&lis);
            return -1;
        }
    }

    //free memory used by the array
    empty_array(&arr);

    //list the content of the list, each time popping one element at its head
    while(lis.structure)
    {
        foreachList(&lis, NULL, Print_dataset);
        printf("Nb of elements: %lu\n", (unsigned long int)lis.nbelements);
        printf("----------------------------------------------------------\n");
        printf("Data with list head purged:\n");
        popListTop(&lis);

    }

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Tests out the insertion at the bottom of a list     */
/*  O :  0 -> Success                                       */
/*      -1 -> Error                                         */
/************************************************************/
int tst_insertbottomlist()
{
    meta_t arr = {NULL, NULL, 20, sizeof(dataset_t), compare_dataset, NULL};
    meta_t lis = {NULL, NULL, 0, sizeof(dataset_t), compare_dataset, NULL};

    printf("/*********************************************************************/\n");
    printf("/********************* tst_insertbottomlist ***************************/\n");
    printf("/*********************************************************************/\n");

    //generate 20 random datasets
    if(setup_data((dataset_t**)&arr.structure, 20) == -1)
    {
        fprintf(stderr, "insertTopList : error while allocating the data\n");
        return -1;
    }

    //display the sorted data
    foreachArray(&arr, NULL, Print_dataset);
    printf("----------------------------------------------------------\n");

    for(int i=0 ; i<20 ; i++)
    {
        if(insertListBottom(&lis, arr.structure + i*sizeof(dataset_t)) == -1)
        {
            fprintf(stderr, "insertBottomList : error while inserting the data\n");
            empty_array(&arr);
            freeDynList(&lis);
            return -1;
        }
    }

    //free memory used by the array
    empty_array(&arr);

    //list the content of the list, each time popping one element at its head
    while(lis.structure)
    {
        foreachList(&lis, NULL, Print_dataset);
        printf("Nb of elements: %lu\n", (unsigned long int)lis.nbelements);
        printf("----------------------------------------------------------\n");
        printf("Data with list head purged:\n");
        popListTop(&lis);

    }

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Tests out the insertion in a sorted list            */
/*  O :  0 -> Success                                       */
/*      -1 -> Error                                         */
/************************************************************/
int tst_insertlistsorted()
{
    meta_t arr = {NULL, NULL, 20, sizeof(dataset_t), compare_dataset, NULL};
    meta_t lis = {NULL, NULL, 0, sizeof(dataset_t), compare_dataset, NULL};

    printf("/*********************************************************************/\n");
    printf("/********************* tst_insertlistsorted **************************/\n");
    printf("/*********************************************************************/\n");

    //generate 20 random datasets
    if(setup_data((dataset_t**)&arr.structure, 20) == -1)
    {
        fprintf(stderr, "insertListSorted : error while allocating the data\n");
        return -1;
    }

    //display the sorted data
    foreachArray(&arr, NULL, Print_dataset);
    printf("----------------------------------------------------------\n");

    for(int i=0 ; i<20 ; i++)
    {
        if(insertListSorted(&lis, arr.structure + i*sizeof(dataset_t)) == -1)
        {
            fprintf(stderr, "insertListSorted : error while inserting the data\n");
            empty_array(&arr);
            freeDynList(&lis);
            return -1;
        }
    }

    printf("Data sorted:\n");
    //display the content of the list, and delete it
    foreachList(&lis, NULL, Print_dataset);
    freeDynList(&lis);

    empty_array(&arr);

    return 0;
}


/************************************************************/
/*  I : /                                                   */
/*  P : Tests out the research in a sorted list             */
/*  O :  0 -> Success                                       */
/*      -1 -> Error                                         */
/************************************************************/
int tst_searchlistsorted()
{
    meta_t arr = {NULL, NULL, 20, sizeof(dataset_t), compare_dataset, NULL};
    meta_t lis = {NULL, NULL, 0, sizeof(dataset_t), compare_dataset, NULL};
    meta_t lisSorted = {NULL, NULL, 0, sizeof(dataset_t), compare_dataset, NULL};
    dataset_t *tmp = NULL, *tmp2 = NULL, tmp3 = {-1, "", -1.0};

    printf("/*********************************************************************/\n");
    printf("/********************* tst_searchlistsorted **************************/\n");
    printf("/*********************************************************************/\n");

    //generate 20 random datasets
    if(setup_data((dataset_t**)&arr.structure, 20) == -1)
    {
        fprintf(stderr, "searchlistsorted : error while allocating the data\n");
        return -1;
    }
    arrayToList(&arr, &lis, COPY);

    //display the sorted data
    foreachList(&lis, NULL, Print_dataset);
    printf("----------------------------------------------------------\n");

    //get the 10th list element
    tmp = (dataset_t*)get_listelem(&lis, 10);
    if(tmp){
        printf("Element at index 10 found :\n");
        Print_dataset(tmp, NULL);
    }

    //search for it (non-sorted method)
    tmp2 = (dataset_t*)find_listelem(&lis, tmp);
    if(!tmp2)
        printf("Same element not found with find_listelem\n");

    //search for an element which is not in the list
    tmp2 = (dataset_t*)find_listelem(&lis, &tmp3);
    if(!tmp2)
        printf("Random element not found with find_listelem\n");

    //free the list
    freeDynList(&lis);

    //search for the first element in an empty list
    tmp2 = (dataset_t*)find_listelem(&lis, tmp);
    if(!tmp2)
        printf("First element not found after list emptied\n");

    //sort the array
    if(quickSortArray(&arr, 0, arr.nbelements-1) == -1){
        printf("tst_searchlistsorted : error while sorting the array\n");
        return -1;
    }
    arrayToList(&arr, &lisSorted, REPLACE);

    //get the 10th list element
    tmp = (dataset_t*)get_listelem(&lisSorted, 10);
    if(tmp){
        printf("Element at index 10 found :\n");
        Print_dataset(tmp, NULL);
    }
    tmp2 = (dataset_t*)find_listelemSorted(&lisSorted, tmp);
    if(!tmp2)
        printf("Same element not found with find_listelem\n");

    //free the list
    freeDynList(&lisSorted);

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Tests out the insertion in a sorted list            */
/*  O :  0 -> Success                                       */
/*      -1 -> Error                                         */
/************************************************************/
int tst_removelistsorted()
{
    meta_t arr = {NULL, NULL, 20, sizeof(dataset_t), compare_dataset, NULL};
    meta_t lis = {NULL, NULL, 0, sizeof(dataset_t), compare_dataset, NULL};

    printf("/*********************************************************************/\n");
    printf("/********************* tst_removelistsorted **************************/\n");
    printf("/*********************************************************************/\n");

    //generate 20 random datasets
    if(setup_data((dataset_t**)&arr.structure, 20) == -1)
    {
        fprintf(stderr, "removelistsorted : error while allocating the data\n");
        return -1;
    }

    //transform the array to a list and display the data
    printf("----------------------------------------------------------\n");
    for(int i=0 ; i<20 ; i++)
    {
        if(insertListSorted(&lis, arr.structure + i*sizeof(dataset_t)) == -1)
        {
            fprintf(stderr, "removelistsorted : error while inserting the data\n");
            empty_array(&arr);
            freeDynList(&lis);
            return -1;
        }
    }
    foreachList(&lis, NULL, Print_dataset);

    //remove elements at index 0, 4, last
    removeListSorted(&lis, get_listelem(&lis, 0));
    removeListSorted(&lis, get_listelem(&lis, 4));
    removeListSorted(&lis, lis.last->data);

    //display the content of the list, and delete it
    printf("Data after removing 1st, 5th and last elements:\n");
    foreachList(&lis, NULL, Print_dataset);
    freeDynList(&lis);

    empty_array(&arr);

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Tests out the bubble sort algo on a list            */
/*  O :  0 -> Success                                       */
/*      -1 -> Error                                         */
/************************************************************/
int tst_bubblesortlist()
{
    meta_t arr = {NULL, NULL, 20, sizeof(dataset_t), compare_dataset, NULL};
    meta_t lis = {NULL, NULL, 0, sizeof(dataset_t), compare_dataset, NULL};

    printf("/*********************************************************************/\n");
    printf("/********************* tst_bubblesortlist ****************************/\n");
    printf("/*********************************************************************/\n");

    //generate 20 random datasets
    if(setup_data((dataset_t**)&arr.structure, 20) == -1)
    {
        fprintf(stderr, "insertTopList : error while allocating the data\n");
        return -1;
    }

    //display the sorted data
    foreachArray(&arr, NULL, Print_dataset);
    printf("----------------------------------------------------------\n");

    for(int i=0 ; i<20 ; i++)
    {
        if(insertListTop(&lis, arr.structure + i*sizeof(dataset_t)) == -1)
        {
            fprintf(stderr, "insertTopList : error while inserting the data\n");
            empty_array(&arr);
            freeDynList(&lis);
            return -1;
        }
    }

    //free memory used by the array
    empty_array(&arr);

    //sort the list using the bubble sort and display it
    bubbleSortList(&lis, 3);
    printf("Data with the %d highest elements sorted:\n", 3);
    foreachList(&lis, NULL, Print_dataset);
    printf("----------------------------------------------------------\n");
    bubbleSortList(&lis, lis.nbelements);
    printf("Data fully sorted:\n");
    foreachList(&lis, NULL, Print_dataset);

    //free the memory used by the list
    freeDynList(&lis);
    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Tests out the conversion algorithms on structures   */
/*  O :  0 -> Success                                       */
/*      -1 -> Error                                         */
/************************************************************/
int tst_structuresconversion()
{
    meta_t arr = {NULL, NULL, 20, sizeof(dataset_t), compare_dataset, NULL};
    meta_t lis = {NULL, NULL, 0, sizeof(dataset_t), compare_dataset, NULL};

    printf("/*********************************************************************/\n");
    printf("/********************* tst_structuresconversion **********************/\n");
    printf("/*********************************************************************/\n");

    //generate 20 random datasets
    if(setup_data((dataset_t**)&arr.structure, 20) == -1)
    {
        fprintf(stderr, "insertTopList : error while allocating the data\n");
        return -1;
    }

    //display the sorted data
    foreachArray(&arr, NULL, Print_dataset);
    printf("----------------------------------------------------------\n");

    //convert the array to a list and display it
    if(arrayToList(&arr, &lis, REPLACE) == -1)
    {
        fprintf(stderr, "structureConversion : error while creating the list\n");
        empty_array(&arr);
        freeDynList(&lis);
        return -1;
    }
    printf("List created:\n");
    foreachList(&lis, NULL, Print_dataset);
    printf("array pointer: %p, number of elments: %lu\n", arr.structure, (unsigned long int)arr.nbelements);
    printf("----------------------------------------------------------\n");

    //convert the array to a list and display it
    if(listToArray(&lis, &arr, REPLACE) == -1)
    {
        fprintf(stderr, "structureConversion : error while creating the array\n");
        empty_array(&arr);
        freeDynList(&lis);
        return -1;
    }
    printf("Array created:\n");
    foreachArray(&arr, NULL, Print_dataset);
    printf("list pointer: %p, number of elments: %lu\n", lis.structure, (unsigned long int)lis.nbelements);

    empty_array(&arr);

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Tests out the insertion in an AVL tree              */
/*  O :  0 -> Success                                       */
/*      -1 -> Error                                         */
/************************************************************/
int tst_insertavl()
{
    meta_t arr = {NULL, NULL, 20, sizeof(dataset_t), compare_dataset, NULL};
    meta_t avl = {NULL, NULL, 0, sizeof(dataset_t), compare_dataset, NULL};

    printf("/*********************************************************************/\n");
    printf("/*************************** tst_insertavl ***************************/\n");
    printf("/*********************************************************************/\n");

    //generate 20 random datasets
    if(setup_data((dataset_t**)&arr.structure, 20) == -1)
    {
        fprintf(stderr, "insertTopList : error while allocating the data\n");
        return -1;
    }

    //display the sorted data
    foreachArray(&arr, NULL, Print_dataset);
    printf("----------------------------------------------------------\n");
    arrayToAVL(&arr, &avl, REPLACE);
    display_AVL_tree(&avl, avl.structure, 'T', toString_dataset, printf);

    empty_array(&arr);
    while(avl.structure)
        delete_AVL_root(&avl);

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Tests out the removal from an AVL tree              */
/*  O :  0 -> Success                                       */
/*      -1 -> Error                                         */
/************************************************************/
int tst_removeavl()
{
    meta_t arr = {NULL, NULL, 20, sizeof(dataset_t), compare_dataset, NULL};
    meta_t avl = {NULL, NULL, 0, sizeof(dataset_t), compare_dataset, NULL};

    printf("/*********************************************************************/\n");
    printf("/*************************** tst_removeavl ***************************/\n");
    printf("/*********************************************************************/\n");

    //generate 20 random datasets
    if(setup_data((dataset_t**)&arr.structure, 20) == -1)
    {
        fprintf(stderr, "removeAVL : error while allocating the data\n");
        return -1;
    }

    arrayToAVL(&arr, &avl, REPLACE);
    display_AVL_tree(&avl, avl.structure, 'T', toString_dataset, printf);
    printf("----------------------------------------------------------\n");

    while(avl.structure)
    {
        delete_AVL_root(&avl);
        display_AVL_tree(&avl, avl.structure, 'T', toString_dataset, printf);
        printf("nb of elements remaining: %lu\n", (unsigned long int)avl.nbelements);
        printf("----------------------------------------------------------\n");
    }

    empty_array(&arr);

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Tests out the research in an AVL tree               */
/*  O :  0 -> Success                                       */
/*      -1 -> Error                                         */
/************************************************************/
int tst_searchavl()
{
    meta_t arr = {NULL, NULL, 20, sizeof(dataset_t), compare_dataset, NULL};
    meta_t avl = {NULL, NULL, 0, sizeof(dataset_t), compare_dataset, NULL};
    dataset_t *tmp = NULL;

    printf("/*********************************************************************/\n");
    printf("/*************************** tst_searchavl ***************************/\n");
    printf("/*********************************************************************/\n");

    //generate 20 random datasets
    if(setup_data((dataset_t**)&arr.structure, 20) == -1)
    {
        fprintf(stderr, "searchAVL : error while allocating the data\n");
        return -1;
    }

    arrayToAVL(&arr, &avl, COPY);
    printf("----------------------------------------------------------\n");

    foreachAVL(&avl, avl.structure, NULL, Print_dataset);
    printf("Nb of elements: %lu\n", (unsigned long int)avl.nbelements);
    tmp = search_AVL(&avl, avl.structure, get_arrayelem(&arr, 5));
    if(!tmp)
        printf("Element %s not found\n", toString_dataset(get_arrayelem(&arr, 5)));
    else
        printf("Element %s found\n", toString_dataset(tmp));

    empty_array(&arr);
    while(avl.structure)
        delete_AVL_root(&avl);

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Tests out the insertion in a queue                  */
/*  O :  0 -> Success                                       */
/*      -1 -> Error                                         */
/************************************************************/
int tst_insertqueue()
{
    meta_t arr = {NULL, NULL, 20, sizeof(dataset_t), compare_dataset, NULL};
    meta_t queue = {NULL, NULL, 0, sizeof(dataset_t), compare_dataset, NULL};

    printf("/*********************************************************************/\n");
    printf("/************************** tst_insertqueue **************************/\n");
    printf("/*********************************************************************/\n");

    //generate 20 random datasets
    if(setup_data((dataset_t**)&arr.structure, 20) == -1)
    {
        fprintf(stderr, "tst_insertqueue : error while allocating the data\n");
        return -1;
    }

    //display the sorted data
    foreachArray(&arr, NULL, Print_dataset);
    printf("----------------------------------------------------------\n");

    printf("Insert all array elements in queue :\n");
    for(uint32_t i = 0 ; i < arr.nbelements ; i++)
        pushQueue(&queue, get_arrayelem(&arr, i));

    foreachQueue(&queue, NULL, Print_dataset);

    printf("Pop elements from queue, one by one :\n");
    while(queue.nbelements){
        Print_dataset(getdata(popQueue(&queue)), NULL);
    }

    if(queue.nbelements)
        printf("tst_insertqueue : Error : %u elements remaining in the queue", queue.nbelements);
    else
        printf("tst_insertqueue : All elements properly popped from the queue");

    empty_array(&arr);

    return 0;
}

/************************************************************/
/*  I : /                                                   */
/*  P : Tests out the insertion in a stack                  */
/*  O :  0 -> Success                                       */
/*      -1 -> Error                                         */
/************************************************************/
int tst_insertstack()
{
    meta_t arr = {NULL, NULL, 20, sizeof(dataset_t), compare_dataset, NULL};
    meta_t stack = {NULL, NULL, 0, sizeof(dataset_t), compare_dataset, NULL};

    printf("/*********************************************************************/\n");
    printf("/************************** tst_insertstack **************************/\n");
    printf("/*********************************************************************/\n");

    //generate 20 random datasets
    if(setup_data((dataset_t**)&arr.structure, 20) == -1)
    {
        fprintf(stderr, "tst_insertstack : error while allocating the data\n");
        return -1;
    }

    //display the sorted data
    foreachArray(&arr, NULL, Print_dataset);
    printf("----------------------------------------------------------\n");

    printf("Insert all array elements in stack :\n");
    for(uint32_t i = 0 ; i < arr.nbelements ; i++)
        pushStack(&stack, get_arrayelem(&arr, i));

    foreachStack(&stack, NULL, Print_dataset);

    printf("Pop elements from stack, one by one :\n");
    while(stack.nbelements){
        Print_dataset(getdata(popStack(&stack)), NULL);
    }

    if(stack.nbelements)
        printf("tst_insertstack : Error : %u elements remaining in the stack", stack.nbelements);
    else
        printf("tst_insertstack : All elements properly popped from the stack");

    empty_array(&arr);

    return 0;
}
