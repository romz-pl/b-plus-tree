
add_executable(proj-a
bplustree.cpp
internalmapping.cpp
internalnode.cpp
key.cpp
leafmapping.cpp
leafnode.cpp
main.cpp
node.cpp
record.cpp
value.cpp
)

# target_compile_options( proj-a PRIVATE -Wall -pedantic -pthread -g -O0 -fprofile-arcs -ftest-coverage -std=c++11 -DNDEBUG )

target_compile_options( proj-a PRIVATE -Wall -pedantic -pthread -std=c++11 -DNDEBUG )
