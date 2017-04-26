/*
 * Using a playback file generated by the python file in this dir,
 * play back the game move by move.
 * 
 * Author: Evan Rittenhouse
 */

#include <stdio.h>
#include <stdlib.h>
#include "../board.h"

int main(int numArgs, char** args)
{
    if (numArgs < 2)
    {
        printf("Please provide the playback file you want to play back.\n");
        return 1;
    }

    FILE *playbackFile;
    int pos[2];
    struct board gameBoard;

    playbackFile = fopen(args[1], "r");

    if (playbackFile == NULL)
    {
        printf("Could not open file %s!\n", args[1]);
        return 1;
    }

    initBoard(&gameBoard);
    printf("Initial board\n\n");
    printBoard(&gameBoard);
    printf("\nPress [ENTER] to continue...");
    getchar();
    system("clear");

    // Start reading the file
    while (!feof(playbackFile))
    {
        fscanf(playbackFile, "(%d, %d)\n", pos, pos + 1);

        if (pos[0] == -1 || pos[1] == -1)
        {
            printf("Player %d cannot move.\n", gameBoard.whoseMove);
            gameBoard.whoseMove = gameBoard.whoseMove ^ 3;
        }
        else
        {
            printf("Player %d plays at (%d, %d)\n\n", gameBoard.whoseMove, pos[0], pos[1]);
            placePiece(&gameBoard, pos);
        }

        printBoard(&gameBoard);

        printf("\nPress [ENTER] to continue...");
        getchar();
        system("clear");
    }

    fclose(playbackFile);
    return 0;
}
