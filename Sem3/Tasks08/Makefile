CPPFLAGS = -std=c++11 -O3 -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic\
 -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=2 -Wwrite-strings\
  -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-property-attribute-mismatch

all: a01.out a02.out a03.out a04.out a05.out a06.out a07.out

a%.out: main_%.o functions.o
	g++ $(CPPFLAGS) $^ -o $@ -lm

main_%.o: main_%.cpp io.h student.h tree.h
	g++ -c $< $(CPPFLAGS) -o $@

functions.o: functions.cpp io.h
	g++ -c functions.cpp $(CPPFLAGS) -o functions.o

clean:
	rm -f *.o *.out my_results
