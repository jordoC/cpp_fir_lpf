cmake_minimum_required (VERSION 3.5)

#Monte Carlo hyperspherical cap intersection sim
project (fir_filter)

# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

# Use C++ 11 standard for compilation
set (CMAKE_CXX_STANDARD 11)

add_subdirectory (csv)
add_subdirectory (fir)
add_subdirectory (tb)
