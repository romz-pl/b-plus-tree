cmake_minimum_required (VERSION 3.0)
enable_language(CXX)
project(btree)

include(CTest)
enable_testing(true)

add_subdirectory(3rdparty/googletest)
add_subdirectory(proj-a/src)
add_subdirectory(proj-b/src)
add_subdirectory(proj-c/src)
add_subdirectory(proj-d/src)
add_subdirectory(proj-e/src)

