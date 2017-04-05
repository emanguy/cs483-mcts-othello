/*
 * This file contains the method definitions for the board structure and associated methods.
 */

#ifndef BOARD_H
#define BOARD_H

struct board 
{
	int spaces[32][32];
	int whoseMove;
	int whiteScore;
	int blackScore;
}

void getMoves(struct board *gameBoard, int *outputArray);
void placePiece(struct board *gameBoard, int *position);
int determineWinner(struct board *gameBoard);
void copyBoard(struct board *source, struct board *dest);
void initBoard(struct board *gameBoard);

#endif

