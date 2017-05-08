#!/usr/bin/fish
time ./monteCarloTreeSearch.out > playback/mcts-st-1.txt
time ./monteCarloTreeSearch.out > playback/mcts-st-2.txt
time ./monteCarloTreeSearch.out > playback/mcts-st-3.txt
mpirun -n 4 ./monteCarloTreeSearch-parallel.out > playback/mcts-mt-1.txt
mpirun -n 4 ./monteCarloTreeSearch-parallel.out > playback/mcts-mt-2.txt
mpirun -n 4 ./monteCarloTreeSearch-parallel.out > playback/mcts-mt-3.txt