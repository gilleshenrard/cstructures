/*
** cstructures.c
** Library regrouping algorithmic-based functions
** Structures available :
**   - Bidirectional lists
**   - Arrays
**   - AVL trees
** ---------------------------------------------------
** Made by Gilles Henrard
** Last modified : 06/01/2021
*/
#include "cstructures.h"

/************************************************************/
/*  I : Metadata of the element to allocate                 */
/*      Actual data of the element to allocate              */
/*  P : Allocates memory for a dynamic element and copy its */
/*          value                                           */
/*  O : Address of the element if OK                        */
/*      NULL if error                                       */
/************************************************************/
dyndata_t* allocate_dyn(meta_t* meta, const void* elem)
{
    dyndata_t* tmp=NULL;

    //memory allocation for the dynamic element
    tmp = calloc(1, sizeof(dyndata_t));
    if(!tmp)
    {
        if(meta->doPError)
            (*meta->doPError)("allocate_dyn: element could not be allocated");

        return NULL;
    }

    //memory allocation for the actual data
    tmp->data = calloc(1, meta->elementsize);
    if(!tmp->data)
    {
        if(meta->doPError)
            (*meta->doPError)("allocate_dyn: element data could not be allocated");

        free(tmp);
        return NULL;
    }

    //copy new element data and set AVL leaf height
    memcpy(tmp->data, elem, meta->elementsize);
    tmp->height = 1;

    return tmp;
}

/************************************************************/
/*  I : Element to deallocate                               */
/*  P : Deallocates the memory for a dynamic element        */
/*  O : /                                                   */
/************************************************************/
int free_dyn(dyndata_t* elem)
{
    free(elem->data);
    free(elem);
    elem = NULL;

    return 0;
}

/************************************************************/
/*  I : Elements to swap                                    */
/*  P : Swaps the pointers of the elements                  */
/*  O : /                                                   */
/************************************************************/
int swap_dyn(dyndata_t* a, dyndata_t* b)
{
    dyndata_t *prev=a->left, *next=b->right;

    a->right = b->right;
    b->left = a->left;
    a->left = b;
    b->right = a;

    if(prev)
        prev->right = b;
    if(next)
        next->left = a;

    return 0;
}

/************************************************************/
/*  I : Element of which get the data                       */
/*  P : Returns the data of the element                     */
/*  O : NULL -> element not allocated                       */
/*      Otherwise -> address of the data element            */
/************************************************************/
void* getdata(dyndata_t* cur)
{
    if(cur)
        return cur->data;
    else
        return NULL;
}

/************************************************************/
/*  I : Element of which get the next one in the structure  */
/*  P : Returns the address of the next element             */
/*  O : NULL -> no element to the right                     */
/*      Otherwise -> address of the next element            */
/************************************************************/
dyndata_t* getright(dyndata_t* cur)
{
    if(cur)
        return cur->right;
    else
        return NULL;
}

/************************************************************/
/*  I : Metadata of the array                               */
/*      index at which retrieve the element                 */
/*  P : Returns the element located at i in the array       */
/*  O : NULL if error                                       */
/*      Address of the element otherwise                    */
/************************************************************/
void* get_arrayelem(meta_t* meta, uint32_t i)
{
    if(i >= meta->nbelements)
    {
        if(meta->doPError)
            (*meta->doPError)("get_arrayelem: no element at the index %" PRIu64 "\n", i);

        return NULL;
    }
    else
        return meta->structure+(meta->elementsize * i);
}

/************************************************************/
/*  I : Metadata of the array                               */
/*      index of the element to which assign a value        */
/*      Value to assign                                     */
/*  P : Set the value for the element located at the index  */
/*          i in the array                                  */
/*  O : -1 if error                                         */
/*      0 otherwise                                         */
/************************************************************/
int set_arrayelem(meta_t* meta, uint32_t i, void* elem)
{
    if(i >= meta->nbelements)
    {
        if(meta->doPError)
            (*meta->doPError)("set_arrayelem: index %" PRIu64 " out of range\n", i);

        return -1;
    }

    memcpy(meta->structure+(meta->elementsize * i), elem, meta->elementsize);
    return 0;
}

/************************************************************/
/*  I : Metadata of the array                               */
/*  P : Free the memory occupied by the array               */
/*  O : -1 if error                                         */
/*      0 otherwise                                         */
/************************************************************/
int empty_array(meta_t* meta){
    //free the array itself
    free(meta->structure);
    meta->structure = NULL;

    //reset rest of the data
    meta->last = NULL;
    meta->nbelements = 0;

    return 0;
}

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

/************************************************************/
/*  I : Array of meta data necessary to the algorithm       */
/*      Number of elements to sort at the end of the array  */
/*  P : Sorts the provided array using the Bubble Sort algo */
/*  O :  0 -> Sorted                                        */
/*      -1 -> Error                                         */
/************************************************************/
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

/************************************************************/
/*  I : Meta data necessary to the algorithm                */
/*      Number of elements to sort at the end of the list   */
/*  P : Sorts the provided linked list using                */
/*          the Bubble Sort algorithm                       */
/*  O :  0 -> Sorted                                        */
/*      -1 -> Error                                         */
/************************************************************/
int bubbleSortList(meta_t* meta, uint32_t nb){
    dyndata_t *current=NULL, *next=NULL, *right_ptr=NULL;
    int swapped = 0;
    uint32_t count = 0;

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

        while(next != right_ptr){
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
        right_ptr = current;
        count ++;
    }while(swapped && count < nb);

    return 0;
}

/************************************************************/
/*  I : Array of meta data necessary to the algorithm       */
/*      Lowest element of the partition                     */
/*      Highest element of the partition                    */
/*  P : Sorts the partitions provided by the Quick Sort     */
/*  O : New pivot                                           */
/************************************************************/
/*  WARNING : is solely to be used by the quick sort func.! */
/************************************************************/
uint32_t quickSortPartitioning(meta_t* meta, uint32_t low, uint32_t high){
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

/************************************************************/
/*  I : Array of meta data necessary to the algorithm       */
/*      Lowest index in the array (most likely 0)           */
/*      Highest index in the array (last element)           */
/*  P : Sorts the provided array using the Quick Sort algo  */
/*  O :  0 -> Sorted                                        */
/*      -1 -> Error                                         */
/************************************************************/
int quickSortArray(meta_t* meta, uint32_t low, uint32_t high){
    uint32_t pivot=0, diffLow = 0, diffHigh = 0;

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

/************************************************************/
/*  I : Meta data necessary to the algorithm                */
/*      Element to search                                   */
/*      Scope of the search (first occurence or any occur.) */
/*  P : Searches the key using the Binary search algorithm  */
/*  O : -1  -> Not found                                    */
/*     >= 0 -> Index of the occurence found in the array    */
/************************************************************/
int binarySearchArray(meta_t *meta, void* toSearch, e_search scope){
    int i=0, j=meta->nbelements-1, m=0, index=-1;
    void *current = NULL;

    while(i<=j)
    {
        m = (i+j)/2;
        //position the cursor
        current = get_arrayelem(meta, m);

        if((*meta->doCompare)(current, toSearch) < 0)
            i = m+1;
        else
        {
            if((*meta->doCompare)(current, toSearch) > 0)
                j = m-1;
            else
            {
                index = m;
                if(scope == FIRST)
                    j = m-1;
                else
                    i = j+1;
            }
        }
    }

    return index;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Index of the element to get                         */
/*  P : Gets the nth element of a list                      */
/*  O : Element if found                                    */
/*      NULL otherwise                                      */
/************************************************************/
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

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Element to append in the list                       */
/*  P : Inserts an element at the top of a linked list      */
/*  O : 0 -> Element added                                  */
/*     -1 -> Error                                          */
/************************************************************/
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

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Element to append in the list                       */
/*  P : Inserts an element at the bottom of a linked list   */
/*  O : 0 -> Element added                                  */
/*     -1 -> Error                                          */
/************************************************************/
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
        meta->last = newElement;
    }
    else
    {
        meta->last->right = newElement;

        //chain up the new element at the end
        newElement->left = meta->last;
        meta->last = newElement;
    }

    //increment the elements counter
    meta->nbelements++;

    return 0;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*  P : Removes the first element of the list               */
/*  O : 0 -> Element popped                                 */
/*     -1 -> Error                                          */
/************************************************************/
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
    free_dyn(head);
    if(second && second->left)
        second->left = NULL;
    meta->structure = second;

    //update the number of elements
    meta->nbelements--;

    return 0;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*  P : Removes the last element of the list                */
/*  O : 0 -> Element popped                                 */
/*     -1 -> Error                                          */
/************************************************************/
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
    free_dyn(tail);
    if(second && second->right)
        second->right = NULL;
    meta->last = second;

    //update the number of elements
    meta->nbelements--;

    return 0;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Element to insert in the list                       */
/*  P : Inserts an element at the right place in a sorted   */
/*          linked list                                     */
/*  O : 0 -> Element added                                  */
/*     -1 -> Error                                          */
/************************************************************/
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
    previous->right = newElement;
    newElement->right = current;

    //update the element count
    meta->nbelements++;

    return 0;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Element to remove from the list                     */
/*  P : Remove an element from a sorted linked list         */
/*  O : 0 -> Element removed                                */
/*     -1 -> Error                                          */
/************************************************************/
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
        free_dyn(current);

        //decrement the amount of elements
        meta->nbelements--;
    }

    return 0;
}

/************************************************************/
/*  I : Dynamic list to free                                */
/*      nullable variable (necessary for compatibility)     */
/*  P : Frees the memory of a list and its data             */
/*  O :  0 if OK                                            */
/*      -1 if error                                         */
/************************************************************/
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
        free_dyn(current);
    }

    return 0;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Parameter for the action to perform                 */
/*      Action to perform                                   */
/*  P : Performs an action on every element of the list     */
/*  O : 0 -> OK                                             */
/*     -1 -> Error                                          */
/************************************************************/
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

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Parameter for the action to perform                 */
/*      Action to perform                                   */
/*  P : Performs an action on every element of the array    */
/*  O : 0 -> OK                                             */
/*     -1 -> Error                                          */
/************************************************************/
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

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Element to insert in the AVL                        */
/*  P : Inserts an element in an AVL                        */
/*  O : AVL root if ok                                      */
/*      NULL otherwise                                      */
/************************************************************/
dyndata_t* insertAVL(meta_t* meta, dyndata_t* avl, void* toAdd){
    int height_left=0, height_right=0, balance=0;

    //if tree is empty
    if(!avl){
        if(!meta->structure)
            meta->nbelements = 0;

        //memory allocation for the new element
        avl = allocate_dyn(meta, toAdd);
        meta->nbelements++;
        return avl;
    }

    //sort whether the new element goes as right or left child
    //  + build a new AVL with the child as root
    if((*meta->doCompare)(avl->data, toAdd) != 0){
        if((*meta->doCompare)(avl->data, toAdd) < 0)
            //to the right
            avl->right = insertAVL(meta, avl->right, toAdd);
        else
            //to the left
            avl->left = insertAVL(meta, avl->left, toAdd);
    }
    else{
        //ignore duplicates (forbidden in AVL trees)
        return avl;
    }

    //get the height of the left and right children AVL
    height_right = (avl->right ? avl->right->height : 0);
    height_left = (avl->left ? avl->left->height : 0);

    //update the current node's height
    avl->height = 1+(height_left > height_right ? height_left : height_right);

    //compute the balance (height difference between left and right)
    if(avl)
        balance = get_AVL_balance(avl);

    //re-balance the tree if necessary
    if(balance < -1){
        // right right case
        if((*meta->doCompare)(avl->right->data, toAdd) < 0){
            return rotate_AVL(avl, LEFT);
        }
        // right left case
        if((*meta->doCompare)(avl->right->data, toAdd) > 0){
            avl->right = rotate_AVL(avl->right, RIGHT);
            return rotate_AVL(avl, LEFT);
        }
    }
    if(balance > 1){
        // left left case
        if((*meta->doCompare)(avl->left->data, toAdd) > 0){
            return rotate_AVL(avl, RIGHT);
        }
        //left right case
        if((*meta->doCompare)(avl->left->data, toAdd) < 0){
            avl->left = rotate_AVL(avl->left, LEFT);
            return rotate_AVL(avl, RIGHT);
        }
    }

    return avl;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Element to browse/display                           */
/*      Character designed to be displayed to indicate      */
/*          node direction compared to its root             */
/*      Method to get the string ID of the node             */
/*  P : Displays an AVL as a tree                           */
/*  O : /                                                   */
/************************************************************/
void display_AVL_tree(meta_t* meta, dyndata_t* avl, char dir, char* (*toString)(void*)){
    char tmp[80]={0};
    int height = 0;
    int nbc_pad = 0;

    if(!avl)
        return;

    height = avl->height;

    offset_max = ++offset > offset_max ? offset : offset_max;

    if(avl){
        display_AVL_tree(meta, avl->right, 'R', toString);

        nbc_pad = LG_MAX - (3 * offset) - strlen((*toString)(avl->data));
        for (int i=0;i<nbc_pad;i++)
            strcat(tmp,".");
        strcat(tmp,(*toString)(avl->data));
        printf("%*c%c %s  (H-%d)  L-%14p  T-%14p  R-%14p\n", 3*offset, '-', dir, tmp, height, (void*)avl->left, (void*)avl, (void*)avl->right);

        display_AVL_tree(meta, avl->left, 'L', toString);
    }
    offset--;
}

/************************************************************/
/*  I : AVL tree to rotate                                  */
/*      Side of the rotation (LEFT or RIGHT)                */
/*  P : Rotates an AVL to the side required                 */
/*  O : Rotated AVL                                         */
/************************************************************/
dyndata_t* rotate_AVL(dyndata_t* avl, e_rotation side){
    dyndata_t *newTree=NULL, *child=NULL;
    int height_l=0, height_r=0;

    if(side == RIGHT)
    {
        //prepare pointers for the new tree
        newTree = avl->left;
        child = newTree->right;

        //perform right rotation
        newTree->right = avl;
        avl->left = child;
    }
    else
    {
        //prepare pointers for the new tree
        newTree = avl->right;
        child = newTree->left;

        //perform left rotation
        newTree->left = avl;
        avl->right = child;
    }

    //set new height of the previous root
    height_l = (avl->left ? avl->left->height : 0);
    height_r = (avl->right ? avl->right->height : 0);
    avl->height = (height_l > height_r ? height_l : height_r) + 1;

    //set new height of the new root
    height_l = (newTree->left ? newTree->left->height : 0);
    height_r = (newTree->right ? newTree->right->height : 0);
    newTree->height = (height_l > height_r ? height_l : height_r) + 1;

    return newTree;
}

/************************************************************/
/*  I : AVL tree of which to compute the balance            */
/*  P : Computes and returns the balance of an AVL          */
/*  O : Balance                                             */
/************************************************************/
int get_AVL_balance(dyndata_t* avl){
    int height_left=0, height_right=0;

    if(!avl)
        return 0;

    height_right = (avl->right ? avl->right->height : 0);
    height_left = (avl->left ? avl->left->height : 0);

    return height_left - height_right;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Root of the AVL to which perform the action         */
/*      Parameter for the action to perform                 */
/*      Action to perform                                   */
/*  P : Performs an action on every element of the AVL      */
/*  O : 0 -> OK                                             */
/*     -1 -> Error                                          */
/************************************************************/
int foreachAVL(meta_t* meta, dyndata_t* avl, void* parameter, int (*doAction)(void*, void*)){
    int ret = 0;

    if(avl)
    {
        //perform action on left child
        foreachAVL(meta, avl->left, parameter, doAction);

        //perform action on root
        ret = (*doAction)(avl->data, parameter);
        if(ret == -1)
        {
            if(meta->doPError)
                (*meta->doPError)("foreachAVL: action specified returned with an error");
        }

        //perform action on right child
        foreachAVL(meta, avl->right, parameter, doAction);
    }

    return ret;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      AVL in which search for the key                     */
/*      Key to search in the AVL                            */
/*  P : Recursively search for a key in the AVL             */
/*  O : Leaf if found                                       */
/*      NULL otherwise                                      */
/************************************************************/
void* search_AVL(meta_t* meta, dyndata_t* avl, void* key){
    dyndata_t *child = NULL;

    // if found or not existing, return the result
    if(avl == NULL || (*meta->doCompare)(avl->data, key) == 0)
        return (avl ? avl->data : NULL);

    // otherwise, decide whether going to the right or left child
    if((*meta->doCompare)(avl->data, key) < 0)
        child = avl->right;
    else
        child = avl->left;

    // perform the search in the sub-child
    return search_AVL(meta, child, key);
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*      Root of the AVL from which remove an elemnt         */
/*      Key to remove from the AVL                          */
/*  P : Removes an element from the AVL provided            */
/*  O : Leaf if found                                       */
/*      NULL otherwise                                      */
/************************************************************/
dyndata_t* delete_AVL(meta_t* meta, dyndata_t* root, void* key){
    int height_right=0, height_left=0, balance=0;
    dyndata_t *tmp=NULL, *databuf=NULL;

    //if no AVL, skip
    if(!root)
        return root;

    //key in the right subtree
    if((*meta->doCompare)(root->data, key) < 0)
        root->right = delete_AVL(meta, root->right, key);

    //key in the left subtree
    else if((*meta->doCompare)(root->data, key) > 0)
        root->left = delete_AVL(meta, root->left, key);

    //key is found
    else{
        //node with less than 2 children nodes
        if(root->left==NULL || root->right==NULL)
        {
            //get at least one existing child
            tmp = (root->left ? root->left : root->right);

            if(!tmp){
                //no children nodes
                //directly delete the avl
                tmp = root;
                root = NULL;
            }
            else
            {
                //one child node
                //copy the child node in the father node (with addresses of the grand-children)
                //save the data address of the avl, then copy everything from the child to the avl
                databuf = root->data;
                memcpy(root, tmp, sizeof(dyndata_t));
                //recover the data pointer of the father, then copy the data (child -> avl)
                root->data = databuf;
                memcpy(root->data, tmp->data, meta->elementsize);
            }

            //free the memory of the child and update the elements counter
            free_dyn(tmp);
            meta->nbelements--;
        }
        else{
            //2 children nodes
            //copy the data from the child directly at the right of the avl,
            //      then delete said child
            tmp = min_AVL_value(root->right);
            memcpy(root->data, tmp->data, meta->elementsize);
            root->right = delete_AVL(meta, root->right, tmp->data);
        }
    }

    if(!root)
        return root;

    //get the height of the left and right children AVL
    height_right = (root->right ? root->right->height : 0);
    height_left = (root->left ? root->left->height : 0);

    //update the current node's height
    root->height = 1+(height_left > height_right ? height_left : height_right);

    //if still a root, re-balance accordingly
    balance = get_AVL_balance(root);
    if(balance < -1){
        // right right case
        if(get_AVL_balance(root->right) <= 0){
            return rotate_AVL(root, LEFT);
        }
        // right left case
        if(get_AVL_balance(root->right) > 0){
            root->right = rotate_AVL(root->right, RIGHT);
            return rotate_AVL(root, LEFT);
        }
    }
    if(balance > 1){
        // left left case
        if(get_AVL_balance(root->left) >= 0){
            return rotate_AVL(root, RIGHT);
        }
        //left right case
        if(get_AVL_balance(root->right) < 0){
            root->left = rotate_AVL(root->left, LEFT);
            return rotate_AVL(root, RIGHT);
        }
    }

    return root;
}

/************************************************************/
/*  I : Metadata necessary to the algorithm                 */
/*  P : Removes the root from the AVL provided              */
/*  O :  0 if OK                                            */
/*      -1 otherwise                                        */
/************************************************************/
int delete_AVL_root(meta_t* meta){
    dyndata_t* tmp = meta->structure;

    if(meta->structure)
        meta->structure = delete_AVL(meta, meta->structure, tmp->data);

    return 0;
}

/************************************************************/
/*  I : AVL for which find the smallest value               */
/*  P : Finds the subtree with the smallest value           */
/*          (most to the left)                              */
/*  O : Most left node in the subtree                       */
/************************************************************/
dyndata_t* min_AVL_value(dyndata_t* avl){
    dyndata_t* current = avl;

    while(current->left){
        current = current->left;
    }

    return current;
}
