#ifndef GRIDVAR_H
#define GRIDVAR_H

#include <vector>
#include <cstdint>

std::vector<std::vector<std::vector<int>>> fast_grid_variations(std::vector<std::vector<int>> orig_grid, std::pair<int, int> midpoint, int hook_size, int hook_value, int hook_orientation, std::vector<int> row_gcds, std::vector<int> col_gcds);

std::vector<std::vector<int>> distinct_permutations(std::vector<int> input);

#endif // GRIDVAR_H
