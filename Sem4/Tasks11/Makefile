CXX      = g++
CXXFLAGS = -std=c++14 -O3 -fstack-protector-all -g -W -Wall \
           -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors \
           -Wfloat-equal -Wpointer-arith -Wformat-security \
           -Wmissing-format-attribute -Wformat=2 -Wwrite-strings -Wcast-align \
           -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual \
     

all: a01.out a02.out a03.out a04.out a05.out a06.out

a%.out: a%.o solve.o
	$(CXX) $(CXXFLAGS) $^ -o $@

a%.o: main%.cpp solve.h io.h
	$(CXX) -c $< $(CXXFLAGS) -o $@

solve.o: solve.cpp solve.h io.h
	$(CXX) -c $< $(CXXFLAGS) -o $@

clean:
	rm -f *.o *.out

.PHONY: all clean archive