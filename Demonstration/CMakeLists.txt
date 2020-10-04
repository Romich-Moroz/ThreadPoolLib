# CMakeList.txt : CMake project for Program, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
include_directories("../ThreadPoolLib/")
link_directories("../ThreadPoolLib/")

add_executable (Demonstration "main.cpp" )


# TODO: Add tests and install targets if needed.
target_link_libraries(Demonstration ThreadPoolLib)
