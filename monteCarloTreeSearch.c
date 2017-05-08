/*
* Author: Jenna Bennett, Evan Rittenhouse
* Date: May 4, 2017
* File: monteCarloTreeSearch.c
*
* Description: This file contains the code for the main of Othello and the monte Carlo Tree Search
* code that will run the game Othello and determine the best move to be made.
* Compile using:
* make mcts
*/

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include "node.h"
#include "board.h"

void pickMove(struct board* gameBoard, int nodeLimit, int timeLimit, int *move);
int selectByIndex(node* selectFrom);
int actionSize(int* moves);
int expandAllChildren(node* toExpand, int* moveSet);
int simulate(struct board *toSimulate);
void pickRandomMove(int *moves, int movesSize, int pickedMove[2]);

/*
 *main()
 *
 *loops the game until both players are unable to make a move. The loop is exited and a winner
 *is determined.
*/
int main()
{
	int gameContinues = 100;   	//loop value 
	int boardMove[2];			//the boardMove to be made
	int nodeLimit = 70;  		//the node limit to be given to the function
	int timeLimit = 3;		//the time limit to be given to the function
	int winner;					//holds the value for winner
	struct board gameBoard;		//gameboard
	initBoard(&gameBoard);		//initialize the gameBoard
	srand(time(NULL));


	while(gameContinues > 0)
	{
		//picks the move
		printf("Picking move...\n");
		pickMove(&gameBoard,nodeLimit,timeLimit, boardMove);
		
		//check that there is a move to be made
		if(boardMove[0] == -1 && boardMove[1] == -1)
		{

			//Check if other player can move
			//flipMove(gameBoard->whoseMove);
			gameBoard.whoseMove = (gameBoard.whoseMove == 1) ? 2 : 1;

			pickMove(&gameBoard,nodeLimit,timeLimit, boardMove);

			if(boardMove[0] == -1 && boardMove[1] == -1)
			{
				gameContinues =-10;
				winner = determineWinner(&gameBoard);
				if(winner == 0)
				{
					printf("This game was a tie");
				}
				else if(winner == 1)
				{
					printf("This game was won by White");
				}
				else if(winner == 2)
				{
					printf("This game was won by Black");
				}

				printBoard(&gameBoard);
			}	
			//Continue the game
			else
			{
				placePiece(&gameBoard,boardMove);
			}
		}
		else
		{
			placePiece(&gameBoard, boardMove);
		}
	}

	return 0;
}

// Selects a child node from the given node that has the highest UCT
int selectByIndex(node* selectFrom)
{
    node *child;
	int selected = -1, i;
	float bestValue = 0, uctValue;

	//UCT calculation to select next node
	for (i = 0; i < selectFrom->numChildren; i++)
	{
		child = selectFrom->children[i];
		//UCT calculation
		uctValue = child->numWins / (float)child->numSimulations +
				   sqrt(2 * log(selectFrom->numSimulations / (float)child->numSimulations));

		//check if the uct value for each child is better than the one before
		if (selected == -1 || uctValue > bestValue)
		{
			selected = i;
			bestValue = uctValue;
		}
	}

	return selected;
}

// Gets the number of moves in a moves array
int actionSize(int* moves)
{
	int size = 0;

	while(moves[size] != -1)
	{
		size++;
	}

	return size;
}

int expandAllChildren(node* toExpand, int* moveSet)
{
	int numExpanded = 0;

	while (moveSet[numExpanded * 2] != -1)
	{
		expand(toExpand, moveSet + (numExpanded * 2));
		numExpanded++;
	}

	return numExpanded;
}

// Simulates a board till the end of the game
int simulate(struct board *toSimulate)
{
	struct board boardState;

	copyBoard(toSimulate, &boardState);

	int numOfActions;
	int boardMove[2], allActions[800];
	int winner;
	int startingTurn = boardState.whoseMove;

	while (1)
	{
		numOfActions = 0;
		//get board structure from the node call getmoves and for loop with while loop inside
		//placePiece()
		//get the move to be made and play the game until the player wins or loses
		getMoves(&boardState, allActions);

		//make sure that there are still actions to be taken before the game ends
		numOfActions = actionSize(allActions);
		pickRandomMove(allActions, numOfActions, boardMove);

		//player cannot make a move
		if (boardMove[0] == -1 && boardMove[1] == -1)
		{
			//Check if other player can move
			//fix;	flipMove(boardState->whoseMove);
			boardState.whoseMove = (boardState.whoseMove == 1) ? 2 : 1;

			numOfActions = 0;
			//get board structure from the node call getmoves and for loop with while loop inside
			//placePiece()
			//get the move to be made and play the game until the player wins or loses
			getMoves(&boardState, allActions);

			//make sure that there are still actions to be taken before the game ends
			numOfActions = actionSize(allActions);
			pickRandomMove(allActions, numOfActions, boardMove);

			//determine the winner because other player cannot make a move either
			if (boardMove[0] == -1 && boardMove[1] == -1)
			{
				winner = determineWinner(&boardState);

				if (startingTurn == winner)
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
			//Continue the game
			else
			{
				placePiece(&boardState, boardMove);
			}
		}
		else
		{
			placePiece(&boardState, boardMove);
		}
	}
}

// Given a list of possible moves and the size of the moves list, pick a move and put it in pickedMove
void pickRandomMove(int *moves, int movesSize, int pickedMove[2])
{
	int randomValue;

	if (movesSize != 0)
	{
		//Randomly select a move to make
		randomValue = movesSize / 2;
		randomValue = rand() % randomValue;
		pickedMove[0] = moves[randomValue * 2];
		pickedMove[1] = moves[randomValue * 2 + 1];
	}
	else
	{
		pickedMove[0] = -1;
		pickedMove[1] = -1;
	}
}

// Use MCTS to choose the best move.
void pickMove(struct board* gameBoard, int nodeLimit, int timeLimit, int* move)
{
	node * root; 				//root node
	node * parentNode;           //node to hold the parentNode for backPropagation
	node * selected;				//selected value for expansion


	int allActions[800];           //list of all actions returned by getMoves

	double timeElapsed;			//used when calculated time taken to complete a loop
	int numNewNodes = 0;		//keeps track of the number of nodes created
	int numWins = 0; 			//used to tally the wins for backpropagation
	int numSims = 0;			//used to tally simulations for back-propagation
	int i;						//loop value
	int j;						//loop value
  	int index;					//used for finding the index of the move to be made 

	time_t startTime, checkTime; //used to calculate the time that has passed
	unsigned long long duration;
	struct timeval simulateStart, simulateEnd;

	time(&startTime);				//track time limit
	root = malloc(sizeof(node));
	initializeRoot(root, gameBoard);	  // initialize root  

	while(numNewNodes < nodeLimit)
	{
		//1. Select a node using UCT and the functions provided by #1. (See the js implementation of 
		//selection and the js UCT implementation as a guide).

		selected = root;
		while(selected->numChildren != 0)
		{
			selected = selected->children[selectByIndex(selected)];
		}

		getMoves(&selected->board, allActions);

		//2. Expand the selected node using the expand() function (see #1) after choosing a move. 
		//It can be any unplayed move given by getMoves() (see #2).

		numNewNodes += expandAllChildren(selected, allActions);
		printf("Num total expanded nodes: %d\n", numNewNodes);

		//3. Simulate the game board on the new node until the game is complete 

		gettimeofday(&simulateStart, NULL);

		for(j = 0; j < selected->numChildren; j++)
		{
            // Doing this 3 times because I discovered once we hit the middle of the game we're not expanding more than 1 time
			for (i = 0; i < 3; i++)
			{
				numWins = simulate(&selected->children[j]->board);
				selected->children[j]->numWins += numWins;
				selected->children[j]->numSimulations++;
			}
		}

		gettimeofday(&simulateEnd, NULL);
		duration = 1000 * (simulateEnd.tv_sec - simulateStart.tv_sec) + (simulateEnd.tv_usec - simulateStart.tv_usec) / 1000;
		printf("Simulation took %llu milliseconds\n", duration);

		//4. Back-propagate results up to the root node once you get the results of your simulation.
		//This just involves recording progress in the numWins and numSimulations members of nodes and 
		//following parent up the tree till you hit NULL.

		numWins = 0;
		numSims = 0;	
		for(i = 0; i < selected->numChildren; i++)
		{
		 	
	 		numWins += selected->children[i]->numWins;
		 	numSims += selected->children[i]->numSimulations;
		}
		parentNode = selected;
		
		while(parentNode != NULL)
	    {
			// Do this to every parent node as you ascend the tree
			parentNode->numWins += numWins; // Sum of the total wins resulting from the simulations
			parentNode->numSimulations += numSims; // Or whatever variable you use to store the total number of simulations you just did
			parentNode = parentNode->parent;

		 }

		// check how much time has passed
		time(&checkTime);
		timeElapsed = difftime(checkTime, startTime);
		if(timeElapsed > timeLimit)
		{
			break;
		}

		//check if there are any possible moves to make
		if(root->numChildren == 0)
		{
			break;
		}
  	}

    //check that there were moves to be made
    if(root->numChildren != 0)
    {
		//UCT calculation to select next node 
        index = selectByIndex(root);

		move[0] = root->childMoves[index * 2]; // row for the child that was visited the most
		move[1] = root->childMoves[index * 2 + 1]; //col for the child that was visited the most
	}
	else
	{
		move[0] = -1; //root has no children nodes which means there is no moves to be made
		move[1] = -1; 	
	}

	printf("Player: %d,    Move made is: (%d, %d)\n",gameBoard->whoseMove, move[0], move[1]);
  	deconstructTree(root);
}