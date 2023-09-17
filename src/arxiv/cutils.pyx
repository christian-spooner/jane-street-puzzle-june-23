#cython: language_level=3
from gridvar cimport distinct_permutations
from libc.stdlib cimport abs

import numpy as np

cimport cython
cimport numpy as cnp


# Util functions
cdef cnp.int64_t gcd(cnp.int64_t a, cnp.int64_t b):
    while b:
        a, b = b, a % b
    return abs(a)


cpdef cnp.int64_t calculate_gcd(list numbers):
    cdef cnp.int64_t gcd_result = numbers[0]
    for num in numbers[1:]:
        gcd_result = gcd(gcd_result, num)
    return gcd_result


cpdef list concatenate_numbers(list nums):
    cdef list result = []
    cdef cnp.int64_t current_number = 0
    for num in nums:
        if num == 0:
            if current_number != 0:
                result.append(current_number)
                current_number = 0
        else:
            current_number = current_number * 10 + num
    if current_number != 0:
        result.append(current_number)
    return result


cpdef bint check_gcd_single(list elements, cnp.int64_t val):
    cdef cnp.int64_t gcd_val
    concat_el = concatenate_numbers(elements)
    if len(concat_el):
        gcd_val = calculate_gcd(concat_el)
    else:
        gcd_val = 0
    return gcd_val == val


cpdef bint check_gcd(cnp.ndarray[cnp.int64_t, ndim=2] grid, list row_gcds, list col_gcds):
    cdef int i
    cdef cnp.int64_t val, gcd_val
    for i, val in enumerate(row_gcds):
        concat_row = concatenate_numbers(grid[i].tolist())
        if len(concat_row):
            gcd_val = calculate_gcd(concat_row)
        else:
            gcd_val = 0
        if gcd_val != val:
            return False

    for i, val in enumerate(col_gcds):
        concat_col = concatenate_numbers([grid[j, i] for j in range(grid.shape[0])])
        if len(concat_col):
            gcd_val = calculate_gcd(concat_col)
        else:
            gcd_val = 0
        if gcd_val != val:
            return False
    return True


cpdef bint check_connected_region(cnp.ndarray[cnp.int64_t, ndim=2] grid):
    cdef int N = grid.shape[0]
    cdef int i, j, ni, nj, k

    cdef int[4] dx = [-1, 1, 0, 0]
    cdef int[4] dy = [0, 0, -1, 1]

    for i in range(N):
        for j in range(N):
            if grid[i, j] != 0:
                for k in range(4):
                    ni, nj = i + dx[k], j + dy[k]
                    if 0 <= ni < N and 0 <= nj < N and grid[ni, nj] != 0:
                        break
                else:  # no break -> no non-zero neighbour
                    return False
    return True


cpdef bint check_2_by_2(cnp.ndarray[cnp.int64_t, ndim=2] grid):
    cdef int N = grid.shape[0]
    cdef int i, j
    for i in range(N - 1):
        for j in range(N - 1):
            if not (0 in (grid[i, j], grid[i, j + 1], grid[i + 1, j], grid[i + 1, j + 1])):
                return False
    return True


cpdef bint validate_solution(cnp.ndarray[cnp.int64_t, ndim=2] grid, list row_gcds, list col_gcds):
    if not (check_gcd(grid, row_gcds, col_gcds) and check_connected_region(grid) and check_2_by_2(grid)):
        return False
    return True


cpdef bint prune_check(cnp.ndarray[cnp.int64_t, ndim=2] grid):
    cdef int N = grid.shape[0]
    cdef int i, j, ni, nj, k
    cdef int[4] dx = [-1, 1, 0, 0]
    cdef int[4] dy = [0, 0, -1, 1]

    for i in range(N - 1):
        for j in range(N - 1):
            if (0 not in (grid[i, j], grid[i, j + 1], grid[i + 1, j], grid[i + 1, j + 1])) and (-1 not in (grid[i, j], grid[i, j + 1], grid[i + 1, j], grid[i + 1, j + 1])):
                return False
    for i in range(N):
        for j in range(N):
            if grid[i, j] not in (0, -1):
                for k in range(4):
                    ni, nj = i + dx[k], j + dy[k]
                    if 0 <= ni < N and 0 <= nj < N and grid[ni, nj] != 0:
                        break
                else:  # no break -> no non-zero neighbour
                    return False
    return True


cdef dict midpoints_dict = {}


cpdef precompute_sane_midpoints(int N):
    global midpoints_dict
    midpoints_dict = {}
    for hook_size in range(1, N + 1):
        for hook_orientation in range(4):
            for i in range(N):
                for j in range(N):
                    key = (hook_size, hook_orientation)
                    if key not in midpoints_dict:
                        midpoints_dict[key] = []

                    if hook_orientation == 0 and i + hook_size - 1 < N and j + hook_size - 1 < N:
                        midpoints_dict[key].append((i, j))
                    elif hook_orientation == 1 and i + hook_size - 1 < N and j - hook_size + 1 >= 0:
                        midpoints_dict[key].append((i, j))
                    elif hook_orientation == 2 and i - hook_size + 1 >= 0 and j + hook_size - 1 < N:
                        midpoints_dict[key].append((i, j))
                    elif hook_orientation == 3 and i - hook_size + 1 >= 0 and j - hook_size + 1 >= 0:
                        midpoints_dict[key].append((i, j))


# Program functions
@cython.boundscheck(False)
@cython.wraparound(False)
cpdef bint can_place(cnp.ndarray[cnp.int64_t, ndim=2] grid, tuple midpoint, int hook_size, int hook_orientation):
    cdef int N = grid.shape[0]
    cdef int x = midpoint[0]
    cdef int y = midpoint[1]
    cdef int i, j
    if hook_orientation == 0:
        for i in range(hook_size):
            if grid[x + i, y] != -1:
                return False
        for j in range(1, hook_size):
            if grid[x, y + j] != -1:
                return False

    elif hook_orientation == 1:
        for i in range(hook_size):
            if grid[x + i, y] != -1:
                return False
        for j in range(1, hook_size):
            if grid[x, y - j] != -1:
                return False

    elif hook_orientation == 2:
        for i in range(hook_size):
            if grid[x - i, y] != -1:
                return False
        for j in range(1, hook_size):
            if grid[x, y + j] != -1:
                return False

    elif hook_orientation == 3:
        for i in range(hook_size):
            if grid[x - i, y] != -1:
                return False
        for j in range(1, hook_size):
            if grid[x, y - j] != -1:
                return False

    return True

@cython.boundscheck(False)
@cython.wraparound(False)
cpdef list get_possible_midpoints(cnp.ndarray[cnp.int64_t, ndim=2] grid, int hook_size, int hook_orientation):
    cdef int N = grid.shape[0]
    cdef int i, j
    cdef list midpoints = []
    for midpoint in midpoints_dict[(hook_size, hook_orientation)]:
        if can_place(grid, midpoint, hook_size, hook_orientation):
            midpoints.append(midpoint)
    return midpoints


@cython.boundscheck(False)
@cython.wraparound(False)
cdef list get_grid_positions(tuple midpoint, int hook_size, int hook_orientation):
    cdef int x = midpoint[0]
    cdef int y = midpoint[1]
    cdef int dx, dy
    cdef list dx_dy = [(1, 1), (1, -1), (-1, 1), (-1, -1)]
    dx, dy = dx_dy[hook_orientation]

    cdef list p1 = [(x + i * dx, y) for i in range(hook_size)]
    cdef list p2 = [(x, y + i * dy) for i in range(1, hook_size)]

    return p1 + p2


@cython.boundscheck(False)
@cython.wraparound(False)
cpdef list get_grid_variations(cnp.ndarray[cnp.int64_t, ndim=2] orig_grid, tuple midpoint, int hook_size, int hook_value, int hook_orientation, list row_gcds, list col_gcds):
    cdef int N = orig_grid.shape[0]
    cdef list positions = get_grid_positions(midpoint, hook_size, hook_orientation)
    cdef cnp.ndarray[cnp.int64_t, ndim=2] grid = np.copy(orig_grid)
    cdef list permutations = list(distinct_permutations([hook_value] * hook_value + [0] * (hook_size * 2 - 1 - hook_value)))
    cdef list permutation
    cdef int i, x, y
    cdef tuple position
    cdef list variations = []
    cdef list buff = [-1] * N

    for permutation in permutations:
        for i, (x, y) in enumerate(positions):
            grid[x, y] = permutation[i]

        for i in range(N):
            buff[i] = grid[midpoint[0], i]
        if not check_gcd_single(buff, row_gcds[midpoint[0]]):
            continue

        for i in range(grid.shape[0]):
            buff[i] = grid[i, midpoint[1]]
        if not check_gcd_single(buff, col_gcds[midpoint[1]]):
            continue

        variations.append(np.copy(grid))
    return variations


# C++ extension
from gridvar cimport fast_grid_variations
from libcpp.utility cimport pair
from libcpp.vector cimport vector

ctypedef vector[vector[int]] vector2d
ctypedef vector[vector2d] vector3d


def py_fast_grid_variations(orig_grid, midpoint, hook_size, hook_value, hook_orientation, row_gcds, col_gcds):
    cdef pair[int, int] c_midpoint = pair[int, int](midpoint[0], midpoint[1])
    cdef vector3d c_result
    c_result = fast_grid_variations(orig_grid, c_midpoint, hook_size, hook_value, hook_orientation, row_gcds, col_gcds)
    
    python_result = []
    for vec2d in c_result:
        arr = np.array([[int(x) for x in vec] for vec in vec2d], dtype=np.int64)
        python_result.append(arr)
    
    return python_result