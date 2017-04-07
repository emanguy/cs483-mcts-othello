/*
 * This tests the functionality of the board functions.
 */

#include <stdio.h>
#include "board.h"

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

int main()
{
	struct board myBoard, myCopy;
	int moveSet[15], i;
	int twoWayPlaySpot[2] = {13, 16};

	for (i = 0; i < 15; i++)
	{
		moveSet[i] = 0;
	}

	printf("Initializing board...\n\n");
	initBoard(&myBoard);
	printBoard(&myBoard);

	printf("\nRetrieving list of moves...\n\n");
	getMoves(&myBoard, moveSet);

	printf("[ ");
	for (i = 0; i < 15; i++)
	{
		printf("%d ", moveSet[i]);
	}
	printf("]\n\n");

	printf("Playing the first move in the set...\n\n");
	placePiece(&myBoard, moveSet);
	printBoard(&myBoard);

	printf("\nCopying the board...\n\n");
	copyBoard(&myBoard, &myCopy);
	printBoard(&myCopy);

	printf("\nSetting up a 2-way move...\n\n");

	myCopy.spaces[14][15] = 2;
	myCopy.spaces[14][16] = 2;
	myCopy.spaces[16][13] = 1;
	myCopy.blackScore += 2;
	myCopy.whiteScore += 1;
	printBoard(&myCopy);

	printf("\nPlaying the 2 way move...\n\n");

	placePiece(&myCopy, twoWayPlaySpot);
	printBoard(&myCopy);

	printf("\nThe OG board\n\n");

	printBoard(&myBoard);

	return 0;
}
