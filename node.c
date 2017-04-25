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

    copyBoard(&parentNode->board, &newNode->board);
    placePiece(&newNode->board, action);


    // Update parent node
    if ((parentNode->numChildren != 0) && (( parentNode->numChildren & (parentNode->numChildren - 1) ) == 0)){
        parentNode->children = (node **)realloc(parentNode->children, parentNode->numChildren * 2 * sizeof(node*));
    }

    parentNode->children[parentNode->numChildren] = newNode;

    if ((parentNode->numChildren != 0) && (( parentNode->numChildren & (parentNode->numChildren - 1) ) == 0)){
        parentNode->childMoves = (int *)realloc(parentNode->childMoves, parentNode->numChildren * 4 * sizeof(int));
    }

    parentNode->childMoves[parentNode->numChildren * 2] = action[0];
    parentNode->childMoves[parentNode->numChildren * 2 + 1] = action[1];

    parentNode->numChildren += 1;


    return newNode;
}

void deconstructTree(node* currNode){

    int i;

    for (i = 0; i < currNode->numChildren; i++)
    {
        deconstructTree(currNode->children[i]);
    }

    free(currNode->childMoves);
    free(currNode->children);
    free(currNode);
}

