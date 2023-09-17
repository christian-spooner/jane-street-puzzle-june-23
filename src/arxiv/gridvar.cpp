#include "gridvar.hpp"
#include <vector>
#include <cmath>
#include <algorithm>

std::vector<std::pair<int, int>> get_grid_positions(std::pair<int, int> midpoint, int hook_size, int hook_orientation)
{
    int x = midpoint.first;
    int y = midpoint.second;
    int dx, dy;
    std::vector<std::pair<int, int>> dx_dy = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    dx = dx_dy[hook_orientation].first;
    dy = dx_dy[hook_orientation].second;

    std::vector<std::pair<int, int>> p1, p2;

    for (int i = 0; i < hook_size; ++i)
    {
        p1.push_back({x + i * dx, y});
    }

    for (int i = 1; i < hook_size; ++i)
    {
        p2.push_back({x, y + i * dy});
    }

    p1.insert(p1.end(), p2.begin(), p2.end());

    return p1;
}

std::vector<std::vector<int>> distinct_permutations(std::vector<int> input)
{
    std::sort(input.begin(), input.end());
    std::vector<std::vector<int>> permutations;
    do
    {
        permutations.push_back(input);
    } while (std::next_permutation(input.begin(), input.end()));
    return permutations;
}

std::vector<std::vector<int>> distinct_permutations_internal(std::vector<int> input)
{
    std::sort(input.begin(), input.end());
    std::vector<std::vector<int>> permutations;
    do
    {
        permutations.push_back(input);
    } while (std::next_permutation(input.begin(), input.end()));
    return permutations;
}

int gcd(int a, int b)
{
    while (b)
    {
        int temp = a;
        a = b;
        b = temp % b;
    }
    return std::abs(a);
}

int calculate_gcd(const std::vector<int> &numbers)
{
    int gcd_result = numbers[0];
    for (auto num : numbers)
    {
        gcd_result = gcd(gcd_result, num);
    }
    return gcd_result;
}

std::vector<int> concatenate_numbers(const std::vector<int> &nums)
{
    std::vector<int> result;
    int current_number = 0;
    for (auto num : nums)
    {
        if (num == 0)
        {
            if (current_number != 0)
            {
                result.push_back(current_number);
                current_number = 0;
            }
        }
        else
        {
            current_number = current_number * 10 + num;
        }
    }
    if (current_number != 0)
    {
        result.push_back(current_number);
    }
    return result;
}

bool check_gcd_single(const std::vector<int> &elements, int val)
{
    auto concat_el = concatenate_numbers(elements);
    int gcd_val = (concat_el.size() > 0) ? calculate_gcd(concat_el) : 0;
    return gcd_val == val;
}

std::vector<std::vector<std::vector<int>>> fast_grid_variations(std::vector<std::vector<int>> orig_grid, std::pair<int, int> midpoint, int hook_size, int hook_value, int hook_orientation, std::vector<int> row_gcds, std::vector<int> col_gcds)
{
    int N = orig_grid.size();
    std::vector<std::pair<int, int>> positions = get_grid_positions(midpoint, hook_size, hook_orientation);
    std::vector<std::vector<int>> grid = orig_grid;
    std::vector<int> perm_input(hook_value, hook_value);
    perm_input.resize(hook_size * 2 - 1, 0);
    std::vector<std::vector<int>> permutations = distinct_permutations_internal(perm_input);
    std::vector<int> buff(N, -1);
    std::vector<std::vector<std::vector<int>>> variations;

    for (const auto &permutation : permutations)
    {
        for (int i = 0; i < static_cast<int>(positions.size()); ++i)
        {
            grid[positions[i].first][positions[i].second] = permutation[i];
        }

        for (int i = 0; i < N; ++i)
        {
            buff[i] = grid[midpoint.first][i];
        }
        if (!check_gcd_single(buff, row_gcds[midpoint.first]))
        {
            continue;
        }

        for (int i = 0; i < N; ++i)
        {
            buff[i] = grid[i][midpoint.second];
        }
        if (!check_gcd_single(buff, col_gcds[midpoint.second]))
        {
            continue;
        }

        variations.push_back(grid);
    }

    return variations;
}