#!/usr/bin/python3
"""
Using the output from monteCarloTreeSearch.out, this will generate
a .playback file which you can use to play back an MCTS game.

Usage: ./gen_playback_from_output.py FILE_IN FILE_OUT
"""

from sys import argv as arg
import re

if len(arg) < 3:
    print("Please specify the file to read and the file to write.")
    print("Usage: ./gen_playback_from_output.py FILE_IN FILE_OUT")
    exit()

regex = re.compile(r"\(-?\d{1,2}, -?\d{1,2}\)")
with open(arg[1]) as mcts_output:
    with open("{0}.playback".format(arg[2]), mode="w") as playback_output:

        for line in mcts_output:
            result = regex.search(line)

            if result:
                playback_output.write("{0}\n".format(result.group(0)))
