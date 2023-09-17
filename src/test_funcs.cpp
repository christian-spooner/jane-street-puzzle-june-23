#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

#include "funcs.hpp"

int main() {
	std::vector<int> r1 = range(1, 5);
	std::vector<int> expected_r1 = {1, 2, 3, 4};
	assert(r1 == expected_r1);
	std::cout << "Test 1 passed" << std::endl;

	std::vector<int> a = {1, 2, 3, 4, 5, 6};
	std::vector<int> b = {3, 4, 5, 6};
	std::vector<int> diff = set_difference(a, b);
	std::vector<int> expected_diff = {1, 2};
	assert(diff == expected_diff);
	std::cout << "Test 2 passed" << std::endl;

	std::vector<std::vector<int>> grid{{0, 3, 0}, {3, 2, 2}, {3, 0, 1}};
	bool res = prune_check(grid);
	assert(res);
	grid = {{0, 3, 0}, {3, 2, 0}, {3, 2, 1}};
	res = prune_check(grid);
	assert(!res);
	std::cout << "Test 3 passed" << std::endl;

	return 0;
}