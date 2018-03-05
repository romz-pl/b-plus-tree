
#
# Makefile for g++ compiler
#

# Name of GNU compiler
# CXX := g++



CXXWARNINGS := -Wall -Wextra -Wconversion -Wsign-conversion

#Compilation flags
# CXXFLAGS := -std=c++11 -DNDEBUG -O2 -funroll-all-loops -ffast-math -Wall
# CXXFLAGS := -std=c++11 -DNDEBUG -O2 -Wall -mtune=native -march=native
CXXFLAGS := -std=c++11 -O2 -Wall $(CXXWARNINGS)

# Google test directory
GTEST_DIR := ../googletest

# Google Test library
CXXLIB := ${GTEST_DIR}/gtest.a -pthread 

# Google Test header files
CXXINCLUDE += -I${GTEST_DIR}/include 
CXXINCLUDE += -I../../src/

CXXFLAGS += $(CXXINCLUDE)

CPPFLAGS := $(CXXINCLUDE)

# Name of resulted binaries
BINOUT := test.x

# Directory with source code
VPATH := ../src/
#VPATH += ../../src

# Source files (listed in alphabetical order)

SOURCE :=
SOURCE += btree_map_test.cpp
SOURCE += main.cpp


#Object files
OBJECT := $(SOURCE:.cpp=.o)

#Dependency files
DEP := $(SOURCE:.cpp=.d)


$(BINOUT) : $(OBJECT)
	$(CXX) $(CXXFLAGS) $(OBJECT) $(CXXLIB) -o $(BINOUT) 

-include $(DEP)

%.d : %.cpp
	@$(CC) -M $(CPPFLAGS) $< | sed 's#\(.*\)\.o: #\1.o \1\.d: #g' > $@


%.o : %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<


.PHONY : clean


clean :
	rm -f *.o *.d $(BINOUT)


