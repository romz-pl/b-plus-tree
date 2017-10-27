
#
# Makefile for g++ compiler
#

# Name of GNU compiler
# CXX := g++


#Compilation flags
# CXXFLAGS := -std=c++11 -DNDEBUG -O2 -Wall -mtune=native -march=native
CXXFLAGS := -std=c++11 -g -O0 -Wall

# Required libraries 
CXXLIB = 

# Name of resulted binaries
BINOUT := bplustree.x

# Directory with source code
VPATH := ../src/

# Source files (listed in alphabetical order)

SOURCE :=
SOURCE += BPlusTree.cpp
SOURCE += exceptions.cpp
SOURCE += InternalNode.cpp
SOURCE += LeafNode.cpp
SOURCE += main.cpp
SOURCE += Node.cpp
SOURCE += Printer.cpp
SOURCE += Record.cpp


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


