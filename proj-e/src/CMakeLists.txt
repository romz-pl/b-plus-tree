
add_executable(proj-e
main.cpp
)

# target_compile_options( proj-e PRIVATE -Wall -pedantic -pthread -g -O0 -fprofile-arcs -ftest-coverage -std=c++11 -DNDEBUG )

target_compile_options( proj-e PRIVATE -Wall -pedantic -pthread -std=c++11 -DNDEBUG )
