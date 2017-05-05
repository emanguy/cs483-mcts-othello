mcts: monteCarloTreeSearch.c board.c node.c
	gcc -g -Wall board.c node.c monteCarloTreeSearch.c -o monteCarloTreeSearch.out -lm
mcts-parallel: monteCarloTreeSearch-parallel.c board.c node.c
	mpicc -g -Wall board.c node.c monteCarloTreeSearch-parallel.c -o monteCarloTreeSearch-parallel.out -lm
testBoard: testBoard.c board.c
	gcc -g -Wall board.c testBoard.c -o testBoard.out
playback: playback/play-back.c board.c
	gcc -g -Wall board.c playback/play-back.c -o playback/play-back.out
test_work_assignment: test_work_assignment.c board.c mpitimer.o
	mpicc -g -Wall mpitimer.o board.c test_work_assignment.c -o test_work_assignment.out
mpitimer.o:
	mpicc -g -c -Wall mpitimer.c -o mpitimer.o