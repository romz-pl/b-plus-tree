
add_executable(proj-b
bplustree.cpp
internal.cpp
key.cpp
leaf.cpp
main.cpp
node.cpp
value.cpp
)

# target_compile_options( proj-b PRIVATE -Wall -pedantic -pthread -g -O0 -fprofile-arcs -ftest-coverage -std=c++11 -DNDEBUG )

target_compile_options( proj-b PRIVATE -Wall -pedantic -pthread -std=c++11 -DNDEBUG )
