#include "funcs.hpp"

struct Key {
	int hook_size;
	int hook_orientation;

	bool operator<(const Key& other) const {
		return std::tie(hook_size, hook_orientation) <
			   std::tie(other.hook_size, other.hook_orientation);
	};

	bool operator==(const Key& other) const {
		return hook_size == other.hook_size && hook_orientation == other.hook_orientation;
	};
};

struct KeyHash {
	std::size_t operator()(const Key& key) const {
		std::size_t h1 = std::hash<int>{}(key.hook_size);
		std::size_t h2 = std::hash<int>{}(key.hook_orientation);
		return h1 ^ (h2 << 1);
	}
};

struct VectorHash {
	size_t operator()(const std::vector<int>& vec) const {
		size_t seed = vec.size();
		for (const int& i : vec) {
			seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		return seed;
	}
};

struct VectorEqual {
	bool operator()(const std::vector<int>& lhs, const std::vector<int>& rhs) const {
		return lhs == rhs;
	}
};

typedef std::unordered_map<Key, std::vector<std::pair<int, int>>, KeyHash> MidpointsMap;
MidpointsMap midpoints_dict;

// Misc functions
void precompute_sane_midpoints(const int N_) {
	midpoints_dict.clear();
	for (int hook_size = 1; hook_size <= N_; ++hook_size) {
		for (int hook_orientation = 0; hook_orientation < 4; ++hook_orientation) {
			for (int i = 0; i < N_; ++i) {
				for (int j = 0; j < N_; ++j) {
					Key key = {hook_size, hook_orientation};

					if (hook_orientation == 0 && i + hook_size - 1 < N_ && j + hook_size - 1 < N_) {
						midpoints_dict[key].push_back({i, j});
					} else if (hook_orientation == 1 && i + hook_size - 1 < N_ &&
							   j - hook_size + 1 >= 0) {
						midpoints_dict[key].push_back({i, j});
					} else if (hook_orientation == 2 && i - hook_size + 1 >= 0 &&
							   j + hook_size - 1 < N_) {
						midpoints_dict[key].push_back({i, j});
					} else if (hook_orientation == 3 && i - hook_size + 1 >= 0 &&
							   j - hook_size + 1 >= 0) {
						midpoints_dict[key].push_back({i, j});
					}
				}
			}
		}
	}
}

bool prune_check(const std::vector<std::vector<int>>& grid) {
	static int dx[4] = {-1, 1, 0, 0};
	static int dy[4] = {0, 0, -1, 1};

	for (int i = 0; i < N_ - 1; ++i) {
		for (int j = 0; j < N_ - 1; ++j) {
			if ((grid[i][j] != 0 && grid[i][j + 1] != 0 && grid[i + 1][j] != 0 &&
				 grid[i + 1][j + 1] != 0) &&
				(grid[i][j] != -1 && grid[i][j + 1] != -1 && grid[i + 1][j] != -1 &&
				 grid[i + 1][j + 1] != -1)) {
				return false;
			}
		}
	}

	for (int i = 0; i < N_; ++i) {
		for (int j = 0; j < N_; ++j) {
			if (grid[i][j] != 0 && grid[i][j] != -1) {
				for (int k = 0; k < 4; ++k) {
					int ni = i + dx[k];
					int nj = j + dy[k];
					if (0 <= ni && ni < N_ && 0 <= nj && nj < N_ && grid[ni][nj] != 0) {
						break;
					}
					if (k == 3) {  // no break -> no non-zero neighbour
						return false;
					}
				}
			}
		}
	}
	return true;
}

std::vector<int> set_difference(std::vector<int>& b) {
	static std::vector<int> a = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	std::vector<int> diff;
	std::sort(a.begin(), a.end());
	std::sort(b.begin(), b.end());
	std::set_difference(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(diff));
	return diff;
}

// Get possible midpoints
bool check_direction(const std::vector<std::vector<int>>& grid, const int start_x,
					 const int start_y, const int dx, const int dy, const int size) {
	for (int i = 0; i < size; ++i) {
		int x = start_x + dx * i;
		int y = start_y + dy * i;
		if (grid[x][y] != -1) {
			return false;
		}
	}
	return true;
}

bool can_place(const std::vector<std::vector<int>>& grid, const std::pair<int, int>& midpoint,
			   int hook_size, int hook_orientation) {
	static const std::vector<std::pair<int, int>> directions = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

	int x = midpoint.first;
	int y = midpoint.second;

	int dx = directions[hook_orientation].first;
	int dy = directions[hook_orientation].second;

	return check_direction(grid, x, y, dx, 0, hook_size) &&
		   check_direction(grid, x, y, 0, dy, hook_size);
}

std::vector<std::pair<int, int>> get_possible_midpoints(const std::vector<std::vector<int>>& grid,
														int hook_size, int hook_orientation) {
	std::vector<std::pair<int, int>> midpoints;
	const auto& midpoints_vec = midpoints_dict[{hook_size, hook_orientation}];
	for (const auto& midpoint : midpoints_vec) {
		if (can_place(grid, midpoint, hook_size, hook_orientation)) {
			midpoints.push_back(midpoint);
		}
	}
	return midpoints;
}

// Get grid variations
std::unordered_set<std::vector<int>, VectorHash, VectorEqual> distinct_permutations(
	std::vector<int>& input) {
	std::sort(input.begin(), input.end());
	std::unordered_set<std::vector<int>, VectorHash, VectorEqual> permutations;
	do {
		permutations.emplace(input);
	} while (std::next_permutation(input.begin(), input.end()));
	return permutations;
}

std::vector<std::vector<std::vector<int>>> get_grid_variations(
	std::vector<std::vector<int>> grid, const std::pair<int, int>& midpoint, const int hook_size,
	const int hook_value, const int hook_orientation, const std::vector<int>& row_gcds,
	const std::vector<int>& col_gcds) {
	static std::vector<std::pair<int, int>> dx_dy = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
	static std::array<int, N_> buff;

	if (hook_value > hook_size * 2 - 1) {
		return {};
	}

	std::vector<int> perm_input(hook_size * 2 - 1, 0);
	std::fill_n(perm_input.begin(), hook_value, hook_value);

	std::unordered_set<std::vector<int>, VectorHash, VectorEqual> permutations =
		distinct_permutations(perm_input);
	std::vector<std::vector<std::vector<int>>> variations;

	for (const auto& permutation : permutations) {
		for (int i = 0; i < hook_size; ++i) {
			grid[midpoint.first + i * dx_dy[hook_orientation].first][midpoint.second] =
				permutation[i];
		}
		for (int i = 1; i < hook_size; ++i) {
			grid[midpoint.first][midpoint.second + i * dx_dy[hook_orientation].second] =
				permutation[hook_size - 1 + i];
		}

		for (int i = 0; i < N_; ++i) {
			buff[i] = grid[midpoint.first][i];
		}
		if (!check_gcd_single(buff, row_gcds[midpoint.first])) {
			continue;
		}

		for (int i = 0; i < N_; ++i) {
			buff[i] = grid[i][midpoint.second];
		}
		if (!check_gcd_single(buff, col_gcds[midpoint.second])) {
			continue;
		}

		if (!check_divisors(grid, row_gcds, col_gcds)) {
			continue;
		}

		variations.push_back(grid);
	}

	return variations;
}

// Main backtracking algorithm
void unplace_hooks(std::vector<std::vector<int>>& hooks, const int index) {
	for (int i = index; i < hooks.size(); ++i) {
		hooks[i][1] = -1;
		hooks[i][2] = -1;
	}
}

std::vector<std::vector<int>> place_hooks(std::vector<std::vector<int>>& grid,
										  std::vector<std::vector<int>>& hooks, int index) {
	if (!prune_check(grid)) {
		return {};
	}

	if (index == hooks.size()) {
		return check_gcd(grid, ROW_GCDS, COL_GCDS) ? grid : std::vector<std::vector<int>>();
	}

	int hook_size = hooks[index][0];
	int hook_value = hooks[index][1];
	int hook_orientation = hooks[index][2];

	// Iterate through hook values (reset orientation each time)
	if (hook_value == -1) {
		std::vector<int> hook_values_used;
		for (auto& hook : hooks) {
			if (hook[1] != -1) {
				hook_values_used.push_back(hook[1]);
			}
		}
		std::vector<int> hook_values_left = set_difference(hook_values_used);
		std::sort(hook_values_left.begin(), hook_values_left.end(), std::greater<int>());

		for (int i : hook_values_left) {
			hooks[index][2] = -1;
			hooks[index][1] = i;
			std::vector<std::vector<int>> solution = place_hooks(grid, hooks, index);
			if (!solution.empty()) {
				return solution;
			}
		}
		return {};
	}

	// Iterate through hook orientations
	if (hook_orientation == -1) {
		if (hook_value != 1) {
			for (int i = 0; i < 4; ++i) {
				hooks[index][2] = i;
				std::vector<std::vector<int>> solution = place_hooks(grid, hooks, index);
				if (!solution.empty()) {
					return solution;
				}
			}
		} else {  // Only one possible orientation for '1' hook
			hooks[index][2] = 0;
			std::vector<std::vector<int>> solution = place_hooks(grid, hooks, index);
			if (!solution.empty()) {
				return solution;
			}
		}
		return {};
	}

	// Iterate through hook midpoints
	for (auto& midpoint : get_possible_midpoints(grid, hook_size, hook_orientation)) {
		// Iterate through hook value-placement permutations
		std::vector<std::vector<std::vector<int>>> possible_grids = get_grid_variations(
			grid, midpoint, hook_size, hook_value, hook_orientation, ROW_GCDS, COL_GCDS);
		for (auto& possible_grid : possible_grids) {
			std::vector<std::vector<int>> solution = place_hooks(possible_grid, hooks, index + 1);
			if (!solution.empty()) {
				return solution;
			}
			unplace_hooks(hooks, index + 1);
		}
	}
	return {};
}
