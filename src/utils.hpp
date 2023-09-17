#pragma once

#include <array>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

constexpr std::size_t N_ = 9;

bool check_gcd_single(const std::array<int, N_>& elements, int val);

bool check_divisors(const std::vector<std::vector<int>>& grid, const std::vector<int>& row_gcds,
					const std::vector<int>& col_gcds);

bool check_gcd(const std::vector<std::vector<int>>& grid, const std::vector<int>& row_gcds,
			   const std::vector<int>& col_gcds);

int calculate_empty_area_product(const std::vector<std::vector<int>>& grid);

void display_grid(const std::vector<std::vector<int>>& grid);