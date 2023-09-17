#include "utils.hpp"

int gcd(int a, int b) {
	while (b != 0) {
		int temp = b;
		b = a % b;
		a = temp;
	}
	return a;
}

int calculate_gcd(const std::vector<int>& numbers) {
	if (numbers.empty()) {
		return 0;
	}
	int gcd_result = numbers[0];
	for (const int& num : numbers) {
		gcd_result = gcd(gcd_result, num);
	}
	return gcd_result;
}

std::vector<int> concatenate_numbers(const std::array<int, N_>& nums) {
	std::vector<int> result;
	int current_number = 0;

	for (const int& num : nums) {
		if (num == 0) {
			if (current_number != 0) {
				result.push_back(current_number);
				current_number = 0;
			}
		} else {
			current_number = current_number * 10 + num;
		}
	}

	if (current_number != 0) {
		result.push_back(current_number);
	}

	return result;
}

std::vector<int> concatenate_numbers_inc(const std::array<int, N_>& nums) {
	std::vector<int> result;
	int current_number = 0;

	for (const int& num : nums) {
		if (num == 0) {
			if (current_number > 0) {
				result.push_back(current_number);
			}
			current_number = 0;
		} else if (num == -1) {
			current_number = -1;
		} else {
			current_number = current_number * 10 + num;
		}
	}

	if (current_number > 0) {
		result.push_back(current_number);
	}

	return result;
}

bool check_gcd_single(const std::array<int, N_>& elements, int val) {
	std::vector<int> concat_el = concatenate_numbers(elements);
	int gcd = (concat_el.empty()) ? 0 : calculate_gcd(concat_el);

	return (gcd == val);
}

bool is_common_divisor(const std::vector<int>& elements, const int val) {
	for (int num : elements) {
		if (num % val != 0) {
			return false;
		}
	}
	return true;
}

bool check_divisors(const std::vector<std::vector<int>>& grid, const std::vector<int>& row_gcds,
					const std::vector<int>& col_gcds) {
	static std::array<int, N_> buff;

	for (int i = 0; i < row_gcds.size(); ++i) {
		std::copy(grid[i].begin(), grid[i].end(), buff.begin());
		auto conc = concatenate_numbers_inc(buff);
		bool isCom = is_common_divisor(concatenate_numbers_inc(buff), row_gcds[i]);

		if (!is_common_divisor(concatenate_numbers_inc(buff), row_gcds[i])) {
			return false;
		}
	}

	for (int i = 0; i < col_gcds.size(); ++i) {
		for (int j = 0; j < N_; ++j) {
			buff[j] = grid[j][i];
		}

		if (!is_common_divisor(concatenate_numbers_inc(buff), col_gcds[i])) {
			return false;
		}
	}

	return true;
}

bool check_gcd(const std::vector<std::vector<int>>& grid, const std::vector<int>& row_gcds,
			   const std::vector<int>& col_gcds) {
	static std::array<int, N_> buff;

	for (int i = 0; i < row_gcds.size(); ++i) {
		std::copy(grid[i].begin(), grid[i].end(), buff.begin());

		int gcd = (grid[i].empty()) ? 0 : calculate_gcd(concatenate_numbers(buff));

		if (gcd != row_gcds[i]) {
			return false;
		}
	}

	for (int i = 0; i < col_gcds.size(); ++i) {
		for (int j = 0; j < N_; ++j) {
			buff[j] = grid[j][i];
		}

		int gcd = (grid.empty()) ? 0 : calculate_gcd(concatenate_numbers(buff));

		if (gcd != col_gcds[i]) {
			return false;
		}
	}

	return true;
}

bool check_connected_region(const std::vector<std::vector<int>>& grid) {
	static std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

	for (int i = 0; i < N_; ++i) {
		for (int j = 0; j < N_; ++j) {
			if (grid[i][j] != 0) {
				bool hasNonZeroNeighbour = false;
				for (const auto& dir : directions) {
					int ni = i + dir.first;
					int nj = j + dir.second;

					if (ni >= 0 && ni < N_ && nj >= 0 && nj < N_ && grid[ni][nj] != 0) {
						hasNonZeroNeighbour = true;
						break;
					}
				}
				if (!hasNonZeroNeighbour) {
					return false;
				}
			}
		}
	}

	return true;
}

bool check_2_by_2(const std::vector<std::vector<int>>& grid) {
	for (int i = 0; i < N_ - 1; ++i) {
		for (int j = 0; j < N_ - 1; ++j) {
			if (!(grid[i][j] == 0 || grid[i][j + 1] == 0 || grid[i + 1][j] == 0 ||
				  grid[i + 1][j + 1] == 0)) {
				return false;
			}
		}
	}

	return true;
}

bool validate_solution(const std::vector<std::vector<int>>& grid, const std::vector<int>& row_gcds,
					   const std::vector<int>& col_gcds) {
	if (!(check_gcd(grid, row_gcds, col_gcds) && check_connected_region(grid) &&
		  check_2_by_2(grid))) {
		return false;
	}
	return true;
}

int calculate_region_area(const std::vector<std::vector<int>>& grid,
						  std::vector<std::vector<bool>>& visited, int i, int j) {
	if (i < 0 || i >= N_ || j < 0 || j >= N_) {
		return 0;
	}

	if (visited[i][j] || grid[i][j] != 0) {
		return 0;
	}

	visited[i][j] = true;
	int area = 1;

	area += calculate_region_area(grid, visited, i - 1, j);
	area += calculate_region_area(grid, visited, i + 1, j);
	area += calculate_region_area(grid, visited, i, j - 1);
	area += calculate_region_area(grid, visited, i, j + 1);

	return area;
}

int calculate_empty_area_product(const std::vector<std::vector<int>>& grid) {
	int empty_area_product = 1;

	std::vector<std::vector<bool>> visited(N_, std::vector<bool>(N_, false));

	for (int i = 0; i < N_; ++i) {
		for (int j = 0; j < N_; ++j) {
			if (grid[i][j] == 0 && !visited[i][j]) {
				int area = calculate_region_area(grid, visited, i, j);
				empty_area_product *= area;
			}
		}
	}

	return empty_area_product;
}

void display_grid(const std::vector<std::vector<int>>& grid) {
	for (int i = 0; i < N_; ++i) {
		std::string row;
		for (int j = 0; j < N_; ++j) {
			if (grid[i][j] == -1) {
				row += "# ";
			} else {
				row += std::to_string(grid[i][j]) + " ";
			}
		}
		std::cout << row << std::endl;
	}
	std::cout << "----------------" << std::endl;
}