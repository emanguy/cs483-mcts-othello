/*
 * This tests the functionality of the board functions.
 */

#include <stdio.h>
#include "board.h"

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
