# distutils: language = c++
# distutils: sources = gridvar.cpp

from libcpp.pair cimport pair
from libcpp.vector cimport vector


cdef extern from "gridvar.hpp":
    vector[vector[vector[int]]] fast_grid_variations(vector[vector[int]] orig_grid, pair[int, int] midpoint, int hook_size, int hook_value, int hook_orientation, vector[int] row_gcds, vector[int] col_gcds)

cdef extern from "gridvar.hpp":
    vector[vector[int]] distinct_permutations(vector[int] input)