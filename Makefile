mcts: monteCarloTreeSearch.c board.c node.c
	gcc -g -Wall board.c node.c monteCarloTreeSearch.c -o monteCarloTreeSearch.out -lm
testBoard: testBoard.c board.c
	gcc -g -Wall board.c testBoard.c -o testBoard.out
playback: playback/play-back.c board.c
	gcc -g -Wall board.c playback/play-back.c -o playback/play-back.out
