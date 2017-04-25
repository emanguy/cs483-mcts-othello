mcts:
	gcc -g -Wall board.c node.c monteCarloTreeSearch.c -o monteCarloTreeSearch.out -lm
testBoard:
	gcc -g -Wall board.c testBoard.c -o testBoard.out
