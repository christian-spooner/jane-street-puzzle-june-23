#include <cassert>

#include "utils.hpp"

int main() {
	std::vector<int> numbers = {12, 18, 24, 36};
	int result = calculate_gcd(numbers);
	assert(result == 6);
	std::cout << "Test 1 passed" << std::endl;

	numbers = {0, 5, 3, 0, 1, 2};
	std::vector<int> r2 = {53, 12};
	auto r1 = concatenate_numbers(numbers);
	bool areEqual = std::equal(r1.begin(), r1.end(), r2.begin());
	assert(areEqual);
	std::cout << "Test 2 passed" << std::endl;

	numbers = {0, 5, 2, 0, 1, 2};
	bool res = check_gcd_single(numbers, 4);
	assert(res);
	numbers = {0, 5, 3, 0, 1, 2};
	res = check_gcd_single(numbers, 4);
	assert(!res);
	std::cout << "Test 3 passed" << std::endl;

	return 0;
}