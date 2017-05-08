/*
 * This is the implementation of board.
 */

#include "board.h"
#include <stdio.h>

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
 * @param followType The value of the space you want to follow (so if I wanted to follow black spaces I'd put 2 for this)
 * @param spaces The physical board part of a board struct
 * @param output The array start position where the resulting row/column may be placed.
 * @return 1 if a space was found, 0 otherwise.
 */
int followPiecesToTarget(int rowDirection, int colDirection, int row, int col, int target, int followType,
		int spaces[32][32], int *output)
{
	while (row >= 0 && row < 32 && col >= 0 && col < 32 && 
			spaces[row][col] != target && spaces[row][col] == followType)
	{
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
	}

	return 0;
}

/*
 * Pass in whose move it is, this function flips it to the other move.
 *
 * @param whoseMove Whose turn it is.
 * @return 1 if 2 was passed, 2 if 1 was passed.
 */
int flipMove(int whoseMove)
{
	return whoseMove ^ 3;
}

void getMoves(struct board *gameBoard, int *outputArray)
{
	int i, j, k, l, outputIndex = 0;

	// Look for player's pieces
	for (i = 0; i < 32; i++)
	{
		for (j = 0; j < 32; j++)
		{
			// If I find the player's piece, search outward in 8 directions to find a spot to play
			if (gameBoard->spaces[i][j] == gameBoard->whoseMove)
			{
				for (k = -1; k <= 1; k++)
				{
					for (l = -1; l <= 1; l++)
					{
						outputIndex += 2 * followPiecesToTarget(k, l, i + k, j + l, 0, 
								flipMove(gameBoard->whoseMove), 
								gameBoard->spaces, outputArray + outputIndex);
					}
				}
			}
		}
	}

	// Add sentinel value at array end
	outputArray[outputIndex] = -1;
	outputArray[outputIndex + 1] = -1;
}

void placePiece(struct board *gameBoard, int *position)
{
	int i, j;
	int result[2];

	// Add one for placing the piece
	if (gameBoard->whoseMove == 1)
	{
		gameBoard->whiteScore++;
	}
	else
	{
		gameBoard->blackScore++;
	}

	// Actually place the piece
	gameBoard->spaces[position[0]][position[1]] = gameBoard->whoseMove;

	for (i = -1; i <= 1; i++)
	{
		for (j = -1; j <= 1; j++)
		{
			// Follow surrounding opposite-colored pieces to a piece of player's color
			if (1 == followPiecesToTarget(i, j, position[0] + i, position[1] + j, 
						gameBoard->whoseMove, flipMove(gameBoard->whoseMove),
						gameBoard->spaces, result))
			{
				// Move off of the already-placed piece
				result[0] -= i;
				result[1] -= j;

				// Flip pieces from the result back to the origin
				while (result[0] != position[0] || result[1] != position[1])
				{
					gameBoard->spaces[result[0]][result[1]] = gameBoard->whoseMove;
					if (gameBoard->whoseMove == 1)
					{
						gameBoard->whiteScore++;
						gameBoard->blackScore--;
					}
					else
					{
						gameBoard->blackScore++;
						gameBoard->whiteScore--;
					}

					result[0] -= i;
					result[1] -= j;
				}
			}
		}
	}

	// Flip whose turn it is
	gameBoard->whoseMove = flipMove(gameBoard->whoseMove);
}

int determineWinner(struct board *gameBoard)
{
	if (gameBoard->whiteScore > gameBoard->blackScore)
	{
		return 1;
	}
	else if (gameBoard->whiteScore < gameBoard->blackScore) 
	{
		return 2;
	}
	else
	{
		return 0;
	}
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
					printf("_ ");
					break;
				case 1:
					printf("● ");
					break;
				case 2:
					printf("○ ");
					break;
			}
		}

		printf("\n");
	}
}