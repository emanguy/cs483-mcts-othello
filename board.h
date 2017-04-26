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
};

/*
 * Initializes the data members of a board struct to become the initial state.
 *
 * @param gameBoard A pointer to the board struct that you want to initialize.
 */
void initBoard(struct board *gameBoard);
/*
 * Copies the contents of the source board to the destination board.
 *
 * @param source A pointer to the board you want to copy from.
 * @param dest A pointer to the board you want to copy to.
 */
void copyBoard(struct board *source, struct board *dest);
/*
 * Determines the possible moves for the current player. Fills outputArray
 * with [row, col, row, col, ... , -1, -1] to denote coordinates where
 * you can play moves. The end of these moves is delimited by -1, -1.
 *
 * @param board The othello board.
 * @param outputArray An array that you want to be filled with potential moves.
 */
void getMoves(struct board *gameBoard, int *outputArray);
/*
 * Places a piece for the player whose turn it is, flips pieces 
 * accordingly. Also swaps whose turn it is.
 *
 * @param gameBoard A pointer to the board to be played upon.
 * @param position A length-2 array saying where to place a piece.
 */
void placePiece(struct board *gameBoard, int *position);
/*
 * Determines the winner based on the white and black score at the end of the
 * game.
 *
 * @param gameBoard The board.
 * @return 1 if white won, 2 if black won, 0 if it's a tie.
 */
int determineWinner(struct board *gameBoard);
/*
 * Prints the board to the command line.
 *
 * @param board A reference to the board to print out
 */
 void printBoard(struct board *board);

#endif

