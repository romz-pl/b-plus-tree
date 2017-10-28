
#
# Makefile for g++ compiler
#

# Name of GNU compiler
# CXX := g++

CXXWARNINGS := -Wall -Wextra -Wconversion -Wsign-conversion

#Compilation flags
# CXXFLAGS := -std=c++11 -DNDEBUG -O2 $(CXXWARNINGS) -mtune=native -march=native
CXXFLAGS := -std=c++11 -g -O0 $(CXXWARNINGS)

# Required libraries 
CXXLIB = 

# Name of resulted binaries
BINOUT := bplustree.x

# Directory with source code
VPATH := ../src/

# Source files (listed in alphabetical order)

SOURCE :=
SOURCE += bplustree.cpp
SOURCE += exceptions.cpp
SOURCE += internalnode.cpp
SOURCE += key.cpp
SOURCE += leafmapping.cpp
SOURCE += leafnode.cpp
SOURCE += main.cpp
SOURCE += node.cpp
SOURCE += record.cpp
SOURCE += value.cpp


#Object files
OBJECT := $(SOURCE:.cpp=.o)

#Dependency files
DEP := $(SOURCE:.cpp=.d)


$(BINOUT) : $(OBJECT)
	$(CXX) $(CXXFLAGS) $(OBJECT) $(CXXLIB) -o $(BINOUT)

-include $(DEP)

%.d : %.cpp
	@$(CC) -MM $(CPPFLAGS) $< | sed 's#\(.*\)\.o: #\1.o \1\.d: #g' > $@


%.o : %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<


.PHONY : clean


clean :
	rm -f *.o *.d $(BINOUT)


