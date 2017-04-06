/*
 * This is the implementation of board.
 */

#include "board.h"

void initBoard(struct board *gameBoard)
{
	int i, j;
	// Fill spaces accordingly
	for (i = 0; i < 32; i++)
	{
		for (j = 0; j < 32; j++)
		{
			// Place black & white pieces at board center
			if ((i == 15 || i == 16) && (j == 15 || j == 16))
			{
				gameBoard->spaces[i][j] = i == j? 1 : 2;
			}
			else // Otherwise no piece placed
			{
				gameBoard->spaces[i][j] = 0;
			}
		}
	}

	// Black goes first
	gameBoard->whoseMove = 2;

	// White and black each have 2 pieces on the board
	gameBoard->whiteScore = 2;
	gameBoard->blackScore = 2;
}

void copyBoard(struct board *source, struct board *dest)
{
	int i,j;

	for (i = 0; i < 32; i++)
	{
		for (j = 0; j < 32; j++)
		{
			dest->spaces[i][j] = source->spaces[i][j];
		}
	}

	dest->whoseMove = source->whoseMove;
	dest->whiteScore = source->whiteScore;
	dest->blackScore = source->blackScore;
}

/*
 * Starting from a specific row/column with a specific direction in mind, follow
 * a piece of the same color till you hit "target" or the end of the board. Return
 * the result of this search as "found something" or "found nothing".
 *
 * @param rowDirection The row direction to follow (-1, 0, or 1)
 * @param colDirection The col direction to follow (-1, 0, or 1)
 * @param row The starting row (0-31)
 * @param col The starting column (0-31)
 * @param target The value of the space you're trying to find
 * @param spaces The physical board part of a board struct
 * @param output The array start position where the resulting row/column may be placed.
 * @return 1 if a space was found, 0 otherwise.
 */
int followPiecesToTarget(int rowDirection, int colDirection, int row, int col, int target,
		int spaces[32][32], int *output)
{
	int piece;

	while (row >= 0 && row < 32 && col >= 0 && col < 32 && spaces[row][col] != target)
	{
		piece = spaces[row][col];
		row += rowDirection;
		col += colDirection;
		
		// Make sure I didn't go outside the boundaries
		if (row < 0 || row >= 32 || col < 0 || col >= 32)
		{
			break;
		}

		if (spaces[row][col] == target) // I've advanced and found the target, return
		{
			output[0] = row;
			output[1] = col;
			return 1;
		}
		else if (spaces[row][col] != piece) // If I switch piece types without finding the target, stop
		{
			break;
		}
	}

	return 0;
}

void getMoves(struct board *gameBoard, int *outputArray)
{
	int i, j, k, outputIndex = 0;

	for (i = 0; i < 32; i++)
	{
		for (j = 0; j < 32; j++)
		{
			if (gameBoard->spaces[i][j] == gameBoard->whoseMove)
			{
				for (k = 0; k < 9; )
			}
		}
	}
}
