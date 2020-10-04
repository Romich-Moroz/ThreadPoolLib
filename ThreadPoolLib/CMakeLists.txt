# CMakeList.txt : CMake project for ThreadPoolLib, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.

add_library(ThreadPoolLib STATIC ThreadPoolLib.cpp)

if (UNIX)
    target_link_libraries(ThreadPoolLib pthread)
endif (UNIX)


# TODO: Add tests and install targets if needed.
