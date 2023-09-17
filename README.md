This was the code I used to solve Jane Street's June 2023 puzzle 'Hooks 9'
https://www.janestreet.com/puzzles/hooks-9-index/

Initially I wrote a python program to solve similar 'hook puzzles' on smaller 
boards based on a recursive/heuristic algorithm, but it became too slow as I 
increased the board size. Even after attempting to use C/C++ extensions to the 
python interpreter, it was still too slow, so I ultimately rewrote it all in C++. 
With aggresive optimisation the program was able to solve the 9x9 problem in 
approximately 5hrs, running on an AMD Ryzen 5 3600 6-Core Processor.

If you know of an easy way in which I could have made this more efficient,
please let me know :)