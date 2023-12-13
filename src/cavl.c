/**
 * @file cavl.c
 * @brief Implement the AVL trees data structure
 * @author Gilles Henrard
 * @date 13/12/2023
 */
#include "cavl.h"
#include <string.h>

//AVL internal functions
static dyndata_t* rotate_AVL(dyndata_t* avl, e_rotation side);
static int get_AVL_balance(dyndata_t* avl);
static dyndata_t* delete_AVL(meta_t* meta, dyndata_t* root, void* key);


/*********************************************************************************************/
/*********************************************************************************************/


/**
 * @brief Insert an element in an AVL
 * 
 * @param meta  Metadata necessary to the algorithm
 * @param avl   AVL tree to which add the element
 * @param toAdd Element to insert in the AVL
 * @return Root of the AVL
 * @retval NULL Failure
 */
dyndata_t* insertAVL(meta_t* meta, dyndata_t* avl, void* toAdd){
    int height_left=0, height_right=0, balance=0;

    if(!meta)
        return NULL;

    //if tree is empty
    if(!avl){
        if(!meta->structure)
            meta->nbelements = 0;

        //memory allocation for the new element
        avl = allocate_dyn(meta, toAdd);
        if(!avl)
            return NULL;

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

/**
 * @brief Displays an AVL as a tree
 * 
 * @param meta      Metadata necessary to the algorithm
 * @param avl       Element to browse/display
 * @param dir       Character designed to be displayed to indicate node direction compared to its root
 * @param toString  Method to get the string ID of the node
 * @param doPrint   Method to print a whole line
 */
void display_AVL_tree(meta_t* meta, dyndata_t* avl, char dir, char* (*toString)(void*), int (*doPrint)(const char*, ...)){
    static const uint8_t LG_MAX = 36U;
    static int offset = 0;
    int offset_max = 0;
    char tmp[80]={0};
    int height = 0;
    int nbc_pad = 0;

    if(!avl)
        return;

    height = avl->height;

    offset_max = ++offset > offset_max ? offset : offset_max;

    display_AVL_tree(meta, avl->right, 'R', toString, doPrint);

    nbc_pad = LG_MAX - (3 * offset) - strlen((*toString)(avl->data));
    for (int i=0;i<nbc_pad;i++)
        strcat(tmp,".");
    strcat(tmp,(*toString)(avl->data));
    (*doPrint)("%*c%c %s  (H-%d)  L-%14p  T-%14p  R-%14p\n", 3*offset, '-', dir, tmp, height, (void*)avl->left, (void*)avl, (void*)avl->right);

    display_AVL_tree(meta, avl->left, 'L', toString, doPrint);

    offset--;
}

/**
 * @brief Rotate an AVL to the side required
 * 
 * @param avl   AVL tree to rotate
 * @param side  Side of the rotation (LEFT or RIGHT)
 * @return      Rotated AVL
 */
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

/**
 * @brief Compute and return the balance of an AVL
 * 
 * @param avl   AVL tree of which to compute the balance
 * @return      Balance
 */
int get_AVL_balance(dyndata_t* avl){
    int height_left=0, height_right=0;

    if(!avl)
        return 0;

    height_right = (avl->right ? avl->right->height : 0);
    height_left = (avl->left ? avl->left->height : 0);

    return height_left - height_right;
}

/**
 * @brief Perform an action on every element of the AVL
 * 
 * @param meta      Metadata necessary to the algorithm
 * @param avl       Root of the AVL to which perform the action
 * @param parameter Parameter for the action to perform
 * @param doAction  Action to perform
 * @retval  0 OK
 * @retval -1 Error
 */
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

/**
 * @brief Recursively search for a key in the AVL
 * 
 * @param meta  Metadata necessary to the algorithm
 * @param avl   AVL in which search for the key
 * @param key   Key to search in the AVL
 * @return Node found
 * @retval NULL Key not found
 */
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

/**
 * @brief Remove an element from the AVL provided
 * 
 * @param meta  Metadata necessary to the algorithm
 * @param root  Root of the AVL from which remove an elemnt
 * @param key   Key to remove from the AVL
 * @return Node found
 * @retval NULL Key not found
 */
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
            free_dyn(&tmp);
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

/**
 * @brief Remove the root from the AVL provided
 * 
 * @param meta Metadata necessary to the algorithm
 * @retval  0 OK
 * @retval -1 Error
 */
int delete_AVL_root(meta_t* meta){
    dyndata_t* tmp = meta->structure;

    if(meta->structure)
        meta->structure = delete_AVL(meta, meta->structure, tmp->data);

    return 0;
}

/**
 * @brief Find the subtree with the smallest value
 * @note This is the node most to the left
 * 
 * @param avl AVL for which find the smallest value
 * @return Most left node in the subtree
 */
dyndata_t* min_AVL_value(dyndata_t* avl){
    dyndata_t* current = avl;

    while(current->left){
        current = current->left;
    }

    return current;
}
