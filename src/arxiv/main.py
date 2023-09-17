# type: ignore


""" The grid above can be partitioned into 9 L-shaped “hooks”. The largest is 9-by-9 (contains 17 squares), 
the next largest is 8-by-8 (contains 15 squares), and so on. The smallest hook is just a single square. 
Find where the hooks are located, and place nine 9's in one of the hooks, eight 8's in another, seven 7's 
in another, and so on.

The filled squares must form a connected region. (Squares are “connected” if they are orthogonally adjacent.) 
Furthermore, every 2-by-2 region must contain at least one unfilled square.

The numbers outside the grid denote the greatest common divisors (GCDs) of the numbers formed by concatenating 
digits in consecutive squares when reading left-to-right (within rows) or top-to-bottom (within columns).

The answer to this puzzle is the product of the areas of the connected groups of empty squares in the 
completed grid. """


import copy
import cProfile
import time
from functools import lru_cache

import cutils
import more_itertools as mit
import numpy as np
from utils import *

# Intial variables
N = 9
ROW_GCDS = [55, 1, 6, 1, 24, 3, 6, 7, 2]
COL_GCDS = [5, 1, 6, 1, 8, 1, 22, 7, 8]


# Program functions
def place_hooks(grid: np.ndarray, hooks, index=0) -> np.ndarray | None:
    if not cutils.prune_check(grid):
        return None

    if index == len(hooks):
        return grid if cutils.check_gcd(grid, ROW_GCDS, COL_GCDS) else None

    hook_size = hooks[index][0]
    hook_value = hooks[index][1]
    hook_orientation = hooks[index][2]

    # Iterate through hook values (reset orientation each time)
    if hook_value == -1:
        hook_values_used = [hook[1] for hook in hooks if hook[1] != -1]
        hook_values_left = sorted(
            set(range(1, N + 1)) - set(hook_values_used), reverse=True
        )
        for i in hook_values_left:
            hooks[index][2] = -1
            hooks[index][1] = i
            solution = place_hooks(grid, hooks, index)
            if solution is not None:
                return solution
        return None

    # Iterate through hook orientations
    if hook_orientation == -1:
        if hook_value != 1:
            for i in range(4):
                hooks[index][2] = i
                solution = place_hooks(grid, hooks, index)
                if solution is not None:
                    return solution
        else:  # Only one possible orientation for '1' hook
            hooks[index][2] = 0
            solution = place_hooks(grid, hooks, index)
            if solution is not None:
                return solution

        return None

    # Iterate through hook midpoints
    for midpoint in cutils.get_possible_midpoints(grid, hook_size, hook_orientation):
        # Iterate through hook value-placement permutations
        possible_grids = cutils.fast_grid_variations(
            grid, midpoint, hook_size, hook_value, hook_orientation, ROW_GCDS, COL_GCDS
        )
        for possible_grid in possible_grids:
            solution = place_hooks(possible_grid, copy.deepcopy(hooks), index + 1)
            if solution is not None:
                return solution

    return None


# Main
def main():
    grid = np.full((N, N), -1, dtype=np.int64)
    hooks = [[i, -1, -1] for i in range(N, 0, -1)]
    hooks[0][1] = 5
    hooks[0][2] = 0
    hooks[1][1] = 8
    hooks[1][2] = 3
    hooks[2][1] = 7
    hooks[2][2] = 3
    cutils.precompute_sane_midpoints(N)

    start_time = time.time()
    solution = place_hooks(grid, hooks)
    end_time = time.time()
    execution_time = end_time - start_time

    if solution is not None:
        display_grid(solution)
        answer = calculate_empty_area_product(solution)
        print("Area:", answer)
    else:
        print("No valid solutions found.")

    print("----------------")
    print("Execution time:", round(execution_time, 2), "seconds")


# Concurrent main
from concurrent.futures import ProcessPoolExecutor, as_completed


def worker(grid, hooks):
    solution = place_hooks(grid, hooks)
    return solution


def generate_hook_variations():
    base_hooks = [[i, -1, -1] for i in range(N, 0, -1)]
    hooks_variations = []

    for i in range(4):
        for j in range(4):
            hooks = base_hooks.copy()
            hooks[0][2] = i
            hooks[1][2] = j
            hooks_variations.append(hooks)

    return hooks_variations


def concurrent_main():
    grid = np.full((N, N), -1)
    hook_variations = generate_hook_variations()

    solution = None
    with ProcessPoolExecutor(max_workers=24) as executor:
        chunk_size = 4  # Number of hook variations per batch
        batches = [
            hook_variations[i : i + chunk_size]
            for i in range(0, len(hook_variations), chunk_size)
        ]

        futures = set()
        for batch in batches:
            batch_futures = {
                executor.submit(worker, grid.copy(), hook) for hook in batch
            }
            futures.update(batch_futures)

        start_time = time.time()

        for future in as_completed(futures):
            solution = future.result()
            if solution is not None:
                for remaining_future in futures:
                    remaining_future.cancel()
                break

    end_time = time.time()
    execution_time = end_time - start_time

    if solution is not None:
        display_grid(solution)
        answer = calculate_empty_area_product(solution)
        print("Area:", answer)
    else:
        print("No valid solutions found.")

    print("----------------")
    print("Execution time:", round(execution_time, 2), "seconds")


import sys

if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "--c":
        concurrent_main()
    elif len(sys.argv) > 1 and sys.argv[1] == "--p":
        cProfile.run("main()", "main.profile")
    elif len(sys.argv) > 1 and sys.argv[1] == "--cp":
        cProfile.run("concurrent_main()", "main.profile")
    else:
        main()
