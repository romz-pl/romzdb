
#
# Makefile for g++ compiler
#

# Name of GNU compiler
# CXX := g++

#Compilation flags
# CXXFLAGS := -std=c++11 -DNDEBUG -O2 -funroll-all-loops -ffast-math -Wall
# CXXFLAGS := -std=c++11 -DNDEBUG -O2 -Wall -mtune=native -march=native
CXXFLAGS := -std=c++11 -g -O -Wall -D_DEBUG

# Google test directory
GTEST_DIR := ../googletest

# Google Test library
CXXLIB := ${GTEST_DIR}/gtest.a -pthread

# Google Test header files
CXXFLAGS += -isystem ${GTEST_DIR}/include -I${GTEST_DIR}
CXXFLAGS += -I../../src

# Name of resulted binaries
BINOUT := test.x

# Directory with source code
VPATH := ../src/
VPATH += ../../src

# Source files (listed in alphabetical order)

SOURCE :=
SOURCE += buffermgr.cpp
SOURCE += buffermgr_test.cpp
SOURCE += diskspacemgr.cpp
SOURCE += diskspacemgr_test.cpp
SOURCE += frame.cpp
SOURCE += frame_test.cpp
SOURCE += heappagehdr.cpp
SOURCE += heappagehdr_test.cpp
SOURCE += main.cpp
SOURCE += page.cpp
SOURCE += page_test.cpp
SOURCE += record.cpp
SOURCE += slot.cpp
SOURCE += unixfile.cpp
SOURCE += unixfile_test.cpp


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


