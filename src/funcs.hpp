#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "constants.hpp"
#include "utils.hpp"

void precompute_sane_midpoints(const int N);

std::vector<std::vector<int>> place_hooks(std::vector<std::vector<int>>& grid,
										  std::vector<std::vector<int>>& hooks, int index);