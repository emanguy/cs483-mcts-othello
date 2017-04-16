/*
* Author: Jenna Bennett
* Date: March 30, 2017
* File: monteCarloTreeSearch.c
*
* Description: This file contains the code for the main of Othello and the monte Carlo Tree Search
* code that will run the game Othello and determine the best move to be made.
*
*/

#include <time.h>
#include <stdlib.h>
#include <stdio.h> 
#include "board.h"
#include "math.h"
#include "node.h"

void pickMove(struct board* gameBoard, int nodeLimit, int timeLimit, int* move)
{
	node *root; 				//root node
	node *parentNode;           //node to hold the parentNode for backPropagation
	node *newNode;              // tempNode for looping
	node *selected;				//selected value for expansion
	node *temp;					//temporary pointer for node
	node *child;				//child node pointer

	struct board * boardState;  //gameboard pointer for use in simulation    


	int * allActions;           //list of all actions returned by getMoves
	int * action;				//action to be used for expansion
	int * boardMove;            // boardMove for simulation

	int randomValue;			//randomValue stores generated value used to pick moves for simulation
	int timeElapsed;			//used when calculated time taken to complete a loop
	int gameContinues;			//used to tell if game is still being simulated 
	int numNewNodes = 0;		//keeps track of the number of nodes created
	int winner;					//used for determining the winner
	int numWins = 0; 			//used to tally the wins for backpropagation
	int numSims = 0;			//used to tally simulations for back-propagation
	int i;						//loop value
	int j;						//loop value
	int sum;					//used for summing the wins and simulations 
  	int index;					//used for finding the index of the move to be made 
  	int flag = 10;				//used if there are no moves to be made by the player
	int numOfActions;			//used to track the number of moves available in simulation phase

	double uctValue;			//used when calculating the node to expand
	double bestValue;			//used to store the best value for expansion
	clock_t startTime, timeCheck; //used to calculate the time that has passed

	startTime = clock();				//track time limit
	initializeRoot(root, gameBoard);	  // initialize root
	newNode = root;  					// temporary newNode
	selected = root;					//initialize selected value for expansion

	while(numNewNodes < nodeLimit && flag > 0)
	{
		gameContinues = 100;	//initialize the simulation phase gameloop value
		bestValue = -1000;		// initialize bestvalue as a negative number
		numOfActions = 0;		//set numOfActions to zero each loop through

		//1. Select a node using UCT and the functions provided by #1. (See the js implementation of 
		//selection and the js UCT implementation as a guide).

		//UCT calculation to select next node 
		for(i = 0; i < newNode->numChildren; i++)
		{
			child = newNode->children[i];
			//UCT calculation
			uctValue = child->numWins / child->numSimulations + 
			sqrt(2 * log(newNode->numSimulations / child->numSimulations));

			//check if the uct value for each child is better than the one before
			if(uctValue > bestValue)
			{
				selected = child;
				bestValue = uctValue;
			}
		} 


		
		getMoves(gameBoard, allActions);
		//2. Expand the selected node using the expand() function (see #1) after choosing a move. 
		//It can be any unplayed move given by getMoves() (see #2).
		for(i = 0; i < 200; i++)
		{
			if(allActions[i] == -1)
			{
				break;
			}

			numOfActions++;
		}
		if(numOfActions != 0)
		{

			for(i = 0; i < numOfActions; i+2)
			{
			
				action[0] = allActions[i];
				action[1] = allActions[i+ 1];
				//You need to implement a choose move here and then send that move as action.
				expand(selected, action);
				numNewNodes++;

			}
		}
		// else
		// {
		// 	move[0] = -1;
		// 	move[1] = -1;
		// 	flag == -100;
		//	break;
		// }

		//3. Simulate the game board on the new node until the game is complete 
		for(j = 0; j < selected->numChildren; j++)
		{
			boardState = selected->board;
			while(gameContinues > 0)
			{
				numOfActions = 0;
				//get board structure from the node call getmoves and for loop with while loop inside
				//placePiece()
				//get the move to be made and play the game until the player wins or loses
				getMoves(boardState, allActions);

				for(i = 0; i < 200; i++)
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
					randomValue = rand() % randomValue + 1;  //may not be plus 1
					boardMove[0] = allActions[randomValue];
					boardMove[1] = allActions[randomValue + 1]; 
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
					flipMove(boardState->whoseMove);
					pickMove(boardState, nodeLimit, timeLimit, boardMove);

					//determine the winner because other player cannot make a move either
					if(boardMove[0] == -1 && boardMove[1] == -1)
					{
						gameContinues = -10;
						winner = determineWinner(boardState);
						if(winner == 0)
						{
							selected->numSimulations += 1;

						}
						else if(winner == 1)
						{
							if(boardState->whoseMove == 1)
							{
								selected->numWins += 1;
							}
							selected->numSimulations += 1;

						}	
						else if(winner == 2)
						{
							if(boardState->whoseMove == 2)
							{
								selected->numWins += 1;
							}
							selected->numSimulations += 1;

						}
					}	
					//Continue the game
					else
					{
						placePiece(boardState, boardMove);
					}
				}
				else
				{	
					placePiece(boardState, boardMove);
				}

			}
		}
		//4. Back-propagate results up to the root node once you get the results of your simulation.
		//This just involves recording progress in the numWins and numSimulations members of nodes and 
		//following parent up the tree till you hit NULL.
		
		 while(newNode->parent !=NULL)
		 {
		 	sum = 0;
		 	numWins = 0;
			numSims = 0;	
		 	parentNode = newNode->parent;
		 	//make sure that the parentNode has children
		 	if(parentNode->numChildren > 0)
		 	{
		 		for(i = 0; i<parentNode->numChildren; i++)
		 		{
		 	
		 			temp = parentNode->children[i];
		 			numWins += temp->numWins;
		 			numSims += temp->numSimulations;
		 		}
		 		sum = numWins + parentNode->numWins;
		 		parentNode->numWins = sum;
		 		sum = numSims + parentNode->numSimulations;
		 		parentNode->numSimulations = sum;
		 	}
		 	newNode = parentNode;

		 }

		// check how much time has passed
		timeElapsed = clock() - startTime;
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
  		//loops to find the node that was most visited
  		for(i = 0; i < root->numChildren; i++)
		{
			child = root->children[i];
			numSims = child->numSimulations;
			//finds the most visited node to choose the move to make
		 	if(numSims > bestValue)
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
  	deconstructTree(root);

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
	int * boardMove;			//the boardMove to be made
	int nodeLimit = 1000;  		//the node limit to be given to the function
	int timeLimit = 3000;		//the time limit to be given to the function
	int winner;					//holds the value for winner
	struct board * gameBoard;	//gameboard
	initBoard(gameBoard);		//initialize the gameBoard

	while(gameContinues > 0)
	{
		//picks the move
		pickMove(gameBoard,nodeLimit,timeLimit, boardMove);
		
		//check that there is a move to be made
		if(boardMove[0] == -1 && boardMove[1] == -1)
		{

			//Check if other player can move
			flipMove(gameBoard->whoseMove);
			pickMove(gameBoard,nodeLimit,timeLimit,boardMove);
			if(boardMove[0] == -1 && boardMove[1] == -1)
			{
				gameContinues =-10;
				winner = determineWinner(gameBoard);
				if(winner == 0)
				{
					print("This game was a tie");
				}
				else if(winner == 1)
				{
					print("This game was won by White");
				}
				else if(winner == 2)
				{
					print("This game was won by Black");
				}
			}	
			//Continue the game
			else
			{
				placePiece(gameBoard,boardMove);
			}
		}
		else
		{
			placePiece(gameBoard, boardMove);
		}
	}

	return 0;
}