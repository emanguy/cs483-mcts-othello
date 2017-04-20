#include "node.h"
#include "board.h"


void initializeRoot(node* root, struct board *boardState){

    // Setting default values
    root->parent = NULL;
    root->childMoves = (int *)malloc(4 * sizeof(int));
    root->numChildren = 0;
    root->numWins = 0;
    copyBoard(boardState, &root->board);
    root->numSimulations = 0;
    root->children = (node **)malloc(2 * sizeof(node*));
}


node* expand(node* parentNode, int* action){

    // Dynamically allocate new struct node
    node* newNode = (node *)malloc(sizeof(node));

    // Setting member variables for new node
    newNode->parent = parentNode;
    newNode->childMoves = (int *)malloc(4 * sizeof(int));
    newNode->numChildren = 0;
    newNode->numWins = 0;
    newNode->numSimulations = 0;
    newNode->children = (node **)malloc(2 * sizeof(node*));
    printf("Mallocing space for new node to have 2 children\n");

    copyBoard(&parentNode->board, &newNode->board);
    placePiece(&newNode->board, action);


    // Update parent node
    if ((parentNode->numChildren != 0) && (parentNode->numChildren & (parentNode->numChildren - 1) == 0)){
        parentNode->children = (node **)realloc(parentNode->children, parentNode->numChildren * 2 * sizeof(node*));
    }

    parentNode->children[parentNode->numChildren] = newNode;

    if ((parentNode->numChildren != 0) && (parentNode->numChildren & (parentNode->numChildren - 1) == 0)){
        parentNode->childMoves = (int *)realloc(parentNode->childMoves, parentNode->numChildren * 4 * sizeof(int));
    }

    parentNode->childMoves[parentNode->numChildren * 2] = action[0];
    parentNode->childMoves[parentNode->numChildren * 2 + 1] = action[1];

    parentNode->numChildren += 1;


    return newNode;
}

void deconstructTree(node* currNode){

    node* currParent = NULL;

    // Go through children to the bottom of the right side of the tree
    if(currNode->numChildren != 0){
        deconstructTree(currNode->children[currNode->numChildren - 1]);
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
	deconstructTree(currParent);
    }

}

