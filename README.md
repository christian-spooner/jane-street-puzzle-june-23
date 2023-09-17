This was the code I used to solve Jane Street's June 2023 puzzle 'Hooks 9'.

Problem: https://www.janestreet.com/puzzles/hooks-9-index/
Solution: https://www.janestreet.com/puzzles/hooks-9-solution/

Initially I wrote a python program to solve similar 'hook puzzles' on smaller
boards based on a recursive/heuristic algorithm. However, as I increased the board
size it rapidly became too slow (due to combinatorial explosion). I tried to add
C/C++ extensions to the python interpreter but the gains were insufficient. I 
therefore rewrote everything in C++ for further performance improvements. After 
aggressive optimisation the program was able to solve the full 9x9 problem in 
approximately 5hrs, running on an AMD Ryzen 5 3600 6-Core Processor.

If you know of an easy way in which I could have made this more efficient,
please let me know :)