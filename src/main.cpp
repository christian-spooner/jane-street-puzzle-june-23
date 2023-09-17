#include "constants.hpp"
#include "funcs.hpp"

int main() {
	std::vector<std::vector<int>> grid(N, std::vector<int>(N, -1));

	std::vector<std::vector<int>> hooks;
	for (int i = N; i > 0; --i) {
		std::vector<int> hook = {i, -1, -1};
		hooks.push_back(hook);
	}

	// initial intuitive guess for hook configuration to reduce compute time
	hooks[0][1] = 5;
	hooks[0][2] = 0;
	hooks[1][1] = 9;
	hooks[1][2] = 0;
	hooks[2][1] = 8;
	hooks[2][2] = 3;
	hooks[3][1] = 7;
	hooks[3][2] = 3;

	precompute_sane_midpoints(N);

	auto start_time = std::chrono::steady_clock::now();

	std::vector<std::vector<int>> solution = place_hooks(grid, hooks, 0);

	auto end_time = std::chrono::steady_clock::now();
	double execution_time = std::chrono::duration<double>(end_time - start_time).count();

	if (!solution.empty()) {
		display_grid(solution);
		int answer = calculate_empty_area_product(solution);
		std::cout << "Area: " << answer << std::endl;
	} else {
		std::cout << "No valid solutions found." << std::endl;
	}

	std::cout << "----------------" << std::endl;
	std::cout << "Execution time: " << execution_time << " seconds" << std::endl;

	return 0;
}
