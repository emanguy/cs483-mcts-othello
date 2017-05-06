/*
 * test_work_assignment.c
 *
 * This program is a test of the dynamic work scheduling system I plan to add to 
 * MCTS for its parallelization. It includes both dynamic work assignment and 
 * defining a new datatype for MPI to send.
 */

#include <mpi.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"

const int WORK = 1;
const int DIE = 2;

void master(int numProcesses, MPI_Datatype boardType);
void slave(int id, MPI_Datatype boardType);

int main()
{
    int id, numProcesses;
    MPI_Datatype boardType;
    MPI_Aint offsets[4];
    int dataSizes[4] = {32*32, 1, 1, 1};
    MPI_Datatype dataTypes[4] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT};

    // Init MPI
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    offsets[0] = offsetof(struct board, spaces);
    offsets[1] = offsetof(struct board, whoseMove);
    offsets[2] = offsetof(struct board, whiteScore);
    offsets[3] = offsetof(struct board, blackScore);

    MPI_Type_create_struct(4, dataSizes, offsets, dataTypes, &boardType); // Create board type for MPI
    MPI_Type_commit(&boardType); // Exposes the board type to the MPI messaging system

    if (id == 0)
    {
        printf("Successfully created the board data type in MPI!\n");
        master(numProcesses, boardType);
    }
    else
    {
        slave(id, boardType);
    }

    MPI_Type_free(&boardType); // Frees MPI's data for the defined type
    MPI_Finalize();
    return 0;
}

void master(int numProcesses, MPI_Datatype boardType)
{
    int numMessages = 20, 
        numSends = 0, 
        numReceives = 0, 
        total = 0, 
        recvBuf, i;
    struct board gameBoard;
    MPI_Status workerStatus;

    initBoard(&gameBoard);

    // Initially send work to all processes
    for (i = 1; i < numProcesses; i++)
    {
        MPI_Send(&gameBoard, 1, boardType, i, WORK, MPI_COMM_WORLD);
        numSends++;

        if (numSends == numMessages)
            break;
    }

    // Retrieve results and assign work from processes that finished early
    while (numReceives < numMessages)
    {
        // Receive result from worker
        MPI_Recv(&recvBuf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &workerStatus);
        printf("Process 0: Got result from process %d", workerStatus.MPI_SOURCE);
        numReceives++;
        total += recvBuf;

        // Assign more work if there is more to be done
        if (numSends < numMessages)
        {
            MPI_Send(&gameBoard, 1, boardType, workerStatus.MPI_SOURCE, WORK, MPI_COMM_WORLD);
            numSends++;
            printf(", assigning more work.\n");
        }
        else 
        {
            printf("\n");
        }
    }

    // Tell worker processes to terminate
    for (i = 1; i < numProcesses; i++)
    {
        // When I get data from a worker, I add its result to total and send it a "die" signal
        printf("Process 0: Work complete, telling process %d to die.\n", i);
        total += recvBuf;
        MPI_Send(NULL, 0, boardType, i, DIE, MPI_COMM_WORLD);
    }

    printf("Complete! Total is %d.\n", total);
}

void slave(int id, MPI_Datatype boardType)
{
    int sendBuf = 5,
        maxNumMoves = 10,
        numMoves,
        possibleMoves[200];
    struct board gameBoard;
    MPI_Status masterStatus;

    srand(time(NULL));

    // Listen for more game boards forever
    while (1)
    {
        printf("Process %d: Waiting for tasks from master...\n", id);
        MPI_Recv(&gameBoard, 1, boardType, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &masterStatus);

        // If master says to die, kill the process
        if (masterStatus.MPI_TAG == DIE)
        {
            break;
        }

        numMoves = rand() % maxNumMoves + 1;
        sendBuf = numMoves;

        printf("Process %d: Received a game board from master! Playing %d moves...\n", id, numMoves);
        // Do some arbitrary work on the board
        while (numMoves > 0)
        {
            numMoves--;
            getMoves(&gameBoard, possibleMoves);

            if (possibleMoves[0] == -1)
            {
                break;
            }

            placePiece(&gameBoard, possibleMoves);
        }

        // Send # moves executed to master
        MPI_Send(&sendBuf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    printf("Process %d: Slave process complete!\n", id);
}