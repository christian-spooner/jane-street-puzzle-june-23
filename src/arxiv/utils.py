import math

import numpy as np


def calculate_gcd(numbers: list[int]) -> int:
    gcd_result = numbers[0]
    for num in numbers[1:]:
        gcd_result = math.gcd(gcd_result, num)
    return gcd_result


def concatenate_numbers(nums: np.ndarray) -> list[int]:
    result = []
    current_number = 0

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


def check_gcd_single(elements: np.ndarray, val) -> bool:
    concat_el = concatenate_numbers(elements)
    if concat_el:
        gcd = calculate_gcd(concat_el)
    else:
        gcd = 0

    if gcd != val:
        return False
    return True


def check_gcd(grid: np.ndarray, row_gcds: list[int], col_gcds: list[int]) -> bool:
    for i, val in enumerate(row_gcds):
        concat_row = concatenate_numbers(np.array(grid[i]))

        if concat_row:
            gcd = calculate_gcd(concat_row)
        else:
            gcd = 0

        if gcd != val:
            return False
    for i, val in enumerate(col_gcds):
        concat_col = concatenate_numbers(np.array([row[i] for row in grid]))

        if concat_col:
            gcd = calculate_gcd(concat_col)
        else:
            gcd = 0

        if gcd != val:
            return False

    return True


def check_connected_region(grid: np.ndarray) -> bool:
    N = len(grid)

    directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]

    for i in range(N):
        for j in range(N):
            if grid[i, j] != 0:
                for dx, dy in directions:
                    ni, nj = i + dx, j + dy

                    if 0 <= ni < N and 0 <= nj < N and grid[ni, nj] != 0:
                        break
                else:  # no break -> no non-zero neighbour
                    return False

    return True


def check_2_by_2(grid: np.ndarray) -> bool:
    N = len(grid)
    for i in range(N - 1):
        for j in range(N - 1):
            if not (
                0 in (grid[i, j], grid[i, j + 1], grid[i + 1, j], grid[i + 1, j + 1])
            ):
                return False

    return True


def validate_solution(
    grid: np.ndarray, row_gcds: list[int], col_gcds: list[int]
) -> bool:
    if not (
        check_gcd(grid, row_gcds, col_gcds)
        and check_connected_region(grid)
        and check_2_by_2(grid)
    ):
        return False

    return True


def calculate_empty_area_product(grid: np.ndarray) -> int:
    empty_area_product = 1
    visited = np.zeros_like(grid, dtype=bool)

    for i in range(grid.shape[0]):
        for j in range(grid.shape[1]):
            if grid[i, j] == 0 and not visited[i, j]:
                area = calculate_region_area(grid, visited, i, j)
                empty_area_product *= area

    return empty_area_product


def calculate_region_area(grid: np.ndarray, visited, i, j):
    if i < 0 or i >= grid.shape[0] or j < 0 or j >= grid.shape[1]:
        return 0

    if visited[i, j] or grid[i, j] != 0:
        return 0

    visited[i, j] = True
    area = 1

    area += calculate_region_area(grid, visited, i - 1, j)
    area += calculate_region_area(grid, visited, i + 1, j)
    area += calculate_region_area(grid, visited, i, j - 1)
    area += calculate_region_area(grid, visited, i, j + 1)

    return area


def display_grid(grid: np.ndarray):
    N = len(grid)
    for i in range(N):
        row = ""
        for j in range(N):
            if grid[i, j] == -1:
                row += "#" + " "
            else:
                row += str(grid[i, j]) + " "
        print(row)
    print("----------------")
