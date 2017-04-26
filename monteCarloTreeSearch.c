/*
* Author: Jenna Bennett
* Date: March 30, 2017
* File: monteCarloTreeSearch.c
*
* Description: This file contains the code for the main of Othello and the monte Carlo Tree Search
* code that will run the game Othello and determine the best move to be made.
* Compile using:
* gcc -lm -Wall -o monteCarloTreeSearch monteCarloTreeSearch.c board.c node.c

*/

#include <time.h>
#include <stdlib.h>
#include <stdio.h> 
#include <math.h>
#include "node.h"
#include "board.h"


void pickMove(struct board* gameBoard, int nodeLimit, int timeLimit, int* move)
{
	node * root; 				//root node
	node * parentNode;           //node to hold the parentNode for backPropagation
	node * newNode;              // tempNode for looping
	node * selected;				//selected value for expansion
	node * child;				//child node pointer

	struct board boardState;  //gameboard pointer for use in simulation    


	int allActions[800];           //list of all actions returned by getMoves
	int action[2];				//action to be used for expansion
	int boardMove[2];            // boardMove for simulation

	int randomValue;			//randomValue stores generated value used to pick moves for simulation
	double timeElapsed;			//used when calculated time taken to complete a loop
	int gameContinues;			//used to tell if game is still being simulated 
	int numNewNodes = 0;		//keeps track of the number of nodes created
	int winner;					//used for determining the winner
	int numWins = 0; 			//used to tally the wins for backpropagation
	int numSims = 0;			//used to tally simulations for back-propagation
	int i;						//loop value
	int j;						//loop value
  	int index;					//used for finding the index of the move to be made 
  	int flag = 10;				//used if there are no moves to be made by the player
	int numOfActions;			//used to track the number of moves available in simulation phase

	double uctValue;			//used when calculating the node to expand
	double bestValue;			//used to store the best value for expansion
	time_t startTime, checkTime; //used to calculate the time that has passed

	time(&startTime);				//track time limit
	root = malloc(sizeof(node));
	initializeRoot(root, gameBoard);	  // initialize root  
	newNode = root;  					// temporary newNode
	selected = root;					//initialize selected value for expansion
	srand(time(NULL));
	while(numNewNodes < nodeLimit && flag > 0)
	{
		gameContinues = 100;	//initialize the simulation phase gameloop value
		bestValue = -1000;		// initialize bestvalue as a negative number
		numOfActions = 0;		//set numOfActions to zero each loop through

		//1. Select a node using UCT and the functions provided by #1. (See the js implementation of 
		//selection and the js UCT implementation as a guide).

		selected = root;
		while(selected->numChildren != 0)
		{
			newNode = selected;
			bestValue = -1000;
			//UCT calculation to select next node 
			for(i = 0; i < newNode->numChildren; i++)
			{
				child = newNode->children[i];
				//UCT calculation
				uctValue = child->numWins / (float)child->numSimulations + 
				sqrt(2 * log(newNode->numSimulations / (float)child->numSimulations));

				//check if the uct value for each child is better than the one before
				if(uctValue > bestValue)
				{
					selected = child;
					bestValue = uctValue;
				}
			} 
		}


		getMoves(&selected->board, allActions);
		//2. Expand the selected node using the expand() function (see #1) after choosing a move. 
		//It can be any unplayed move given by getMoves() (see #2).

		for(i = 0; i < 800; i++)
		{
			if(allActions[i] == -1)
			{
				break;
			}

			numOfActions++;
		}
		if(numOfActions != 0)
		{

			for(i = 0; i < numOfActions; i+=2)
			{
			
				action[0] = allActions[i];
				action[1] = allActions[i+ 1];
				//You need to implement a choose move here and then send that move as action.
				expand(selected, action);
				numNewNodes++;

			}
		}

		//3. Simulate the game board on the new node until the game is complete 

		for(j = 0; j < selected->numChildren; j++)
		{
			copyBoard(&selected->children[j]->board, &boardState);
			gameContinues = 100;
			while(gameContinues > 0)
			{
				numOfActions = 0;
				//get board structure from the node call getmoves and for loop with while loop inside
				//placePiece()
				//get the move to be made and play the game until the player wins or loses
				getMoves(&boardState, allActions);

				for(i = 0; i < 800; i++)
				{
					if(allActions[i] == -1)
					{
						break;
					}

					numOfActions++;
				}
				//make sure that there are still actions to be taken before the game ends
				if(numOfActions != 0)
				{
					//Randomly select a move to make
					randomValue = numOfActions / 2;
					randomValue = rand() % randomValue ;  
					boardMove[0] = allActions[randomValue * 2];
					boardMove[1] = allActions[randomValue * 2 + 1]; 
				}
				else
				{
					boardMove[0] = -1;
					boardMove[1] = -1;
				}
				//player cannot make a move
				if(boardMove[0] == -1 && boardMove[1] == -1)
				{

					//Check if other player can move
					//fix;	flipMove(boardState->whoseMove); 
					selected->children[j]->board.whoseMove = (selected->children[j]->board.whoseMove == 1) ? 2 : 1;

					numOfActions = 0;
					//get board structure from the node call getmoves and for loop with while loop inside
					//placePiece()
					//get the move to be made and play the game until the player wins or loses
					getMoves(&boardState, allActions);

					for(i = 0; i < 800; i++)
					{
						if(allActions[i] == -1)
						{
							break;
						}

						numOfActions++;
					}
					//make sure that there are still actions to be taken before the game ends
					if(numOfActions != 0)
					{
						//Randomly select a move to make
						randomValue = numOfActions / 2;
						randomValue = rand() % randomValue ;  
						boardMove[0] = allActions[randomValue * 2];
						boardMove[1] = allActions[randomValue * 2 + 1]; 
					}
					else
					{
						boardMove[0] = -1;
						boardMove[1] = -1;
					}

					//determine the winner because other player cannot make a move either
					if(boardMove[0] == -1 && boardMove[1] == -1)
					{
						gameContinues = -10;
						winner = determineWinner(&boardState);

						if (selected->children[j]->board.whoseMove == winner)
						{
							selected->children[j]->numWins++;
						}

						selected->children[j]->numSimulations++;
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
		
		while(parentNode !=NULL)
	    {
		
			// Do this to every parent node as you ascend the tree
			parentNode->numWins += numWins; // Sum of the total wins resulting from the simulations
			parentNode->numSimulations += numSims; // Or whatever variable you use to store the total number of simulations you just did
			parentNode = parentNode->parent;

		 }

		time(&checkTime);
		// check how much time has passed
		timeElapsed = difftime(checkTime, startTime);
		if(timeElapsed > timeLimit)
		{

			break;
		}

		//check if there are any possible moves to make
		if(root->numChildren == 0)
		{
			flag =-100;
		}
  	}

  	bestValue = -20;
  	numSims = -20;
    index = -1;
    //check that there were moves to be made
    if(root->numChildren != 0)
    {

		//UCT calculation to select next node 
		for(i = 0; i < root->numChildren; i++)
		{
			child = root->children[i];
			//UCT calculation
			uctValue = child->numWins / (float)child->numSimulations + 
			sqrt(2 * log(root->numSimulations / (float)child->numSimulations));

			//check if the uct value for each child is better than the one before
			if(uctValue > bestValue)
			{
				index = i;
				selected = child;
				bestValue = uctValue;
			}
		} 

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

void printBoard(struct board *board)
{
	int i, j;

	if (board->whoseMove == 1)
	{
		printf("White's turn");
	}
	else
	{
		printf("Black's turn");
	}

	printf("\t| White score: %d\t| Black score: %d\n", board->whiteScore, board->blackScore);

	for (i = 0; i < 32; i++)
	{
		for (j = 0; j < 32; j++)
		{
			switch(board->spaces[i][j])
			{
				case 0:
					printf("* ");
					break;
				case 1:
					printf("w ");
					break;
				case 2:
					printf("b ");
					break;
			}
		}

		printf("\n");
	}
}

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
