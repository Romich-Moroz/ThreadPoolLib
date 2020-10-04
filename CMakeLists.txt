# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("ThreadPoolLib")


# Include sub-projects.
add_subdirectory ("ThreadPoolLib")
add_subdirectory ("Demonstration")


