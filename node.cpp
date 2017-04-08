#include "node.h"
#include "board.h"

void initializeRoot(node* root, struct board *boardState){

    // Setting default values
    root->parent = NULL;
    root->childMoves = (int *)malloc(2 * sizeof(int));
    root->numChildren = 0;
    root->numWins = 0;
    root->board = *boardState;
    root->numSimulations = 0;
    root->children = (node **)malloc(sizeof(node*));
    root->id = 0;
}

node* expand(node* parentNode, int* action, int name){

    // Dynamically allocate new struct node
    node* newNode = (node *)malloc(sizeof(node));

    // Setting member variables for new node
    newNode->parent = parentNode;
    newNode->childMoves = (int *)malloc(2 * sizeof(int));
    newNode->numChildren = 0;
    newNode->numWins = 0;
    newNode->numSimulations = 0;
    newNode->children = (node **)malloc(sizeof(node*));
    newNode->id = name;
    //initBoard(&newNode->currBoard);
    copyBoard(&parentNode->currBoard, &newNode->currBoard);

    // Update parent node
    parentNode->children = (node **)realloc(parentNode->children, (parentNode->numChildren + 1) * sizeof(node*));
    parentNode->children[parentNode->numChildren] = newNode;
    parentNode->childMoves = (int *)realloc(parentNode->childMoves, (parentNode->numChildren + 1) * 2 * sizeof(int));
    parentNode->childMoves[parentNode->numChildren * 2] = action[0];
    parentNode->childMoves[parentNode->numChildren * 2 + 1] = action[1];
    parentNode->numChildren += 1;

    return newNode;
}

// Bootstrap function for recursion
void deconstructTree(node* rootNode){
    deleteNodes(rootNode);
    return;
}

void deleteNodes(node* currNode){

    node* currParent = NULL;

    // Go through children to the bottom of the right side of the tree
    if(currNode->numChildren != 0){
        deleteNodes(currNode->children[currNode->numChildren - 1]);
	return;
    }

    // Update parent
    if (currNode->parent != NULL){
        currNode->parent->numChildren -= 1;
        currNode->parent->children[currNode->parent->numChildren] = NULL;
        currParent = currNode->parent;
    }

    // Delete
    free(currNode->children);
    free(currNode->childMoves);
    free(currNode);
    currNode = NULL;

    if (currParent != NULL){
	deleteNodes(currParent);
    }

}
