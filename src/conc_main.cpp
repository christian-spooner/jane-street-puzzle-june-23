// concurrent alternative to main.cpp in attempt to reduce compute time
// this proved to be ineffective...

#include <atomic>
#include <exception>
#include <mutex>
#include <thread>

#include "constants.hpp"
#include "funcs.hpp"

std::atomic<bool> solutionFound(false);
std::vector<std::vector<int>> solution;
std::mutex mtx;

void findSolutionThread(std::vector<std::vector<int>> grid, std::vector<std::vector<int>>& hooks) {
	std::vector<std::vector<int>> threadSolution = place_hooks(grid, hooks, 0);

	if (!threadSolution.empty()) {
		{
			std::lock_guard<std::mutex> lock(mtx);
			if (!solutionFound) {
				solutionFound = true;
				solution = threadSolution;
			}
		}
	}
}

int main() {
	std::vector<std::vector<int>> grid(N, std::vector<int>(N, -1));

	std::vector<std::vector<int>> hooks;
	for (int i = N; i > 0; --i) {
		std::vector<int> hook = {i, -1, -1};
		hooks.push_back(hook);
	}

	precompute_sane_midpoints(N);

    int numThreads = 8;
	std::vector<std::thread> threads;
	std::vector<std::vector<std::vector<int>>> h(numThreads, hooks);

	for (int i = 0; i < numThreads; ++i) {
		h[i][0][2] = i / 4;
		h[i][1][2] = i % 4;
	}

	auto start_time = std::chrono::steady_clock::now();

	for (int i = 0; i < numThreads; ++i) {
		threads.emplace_back(findSolutionThread, grid, std::ref(h[i]));
	}

	while (!solutionFound) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	while (!solutionFound) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	auto end_time = std::chrono::steady_clock::now();
	double execution_time = std::chrono::duration<double>(end_time - start_time).count();

	if (solutionFound) {
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
