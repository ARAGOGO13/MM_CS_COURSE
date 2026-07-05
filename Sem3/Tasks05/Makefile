CPPFLAGS = -std=c++11 -O3 -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic\
 -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings\
  -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format


all: a01.out a02.out a03.out a04.out a05.out a06.out a07.out a08.out

a%.out: main_%.o functions.o io.o
	g++ $(CPPFLAGS) $^ -o $@ -lm

main_%.o: main_%.cpp functions.h io.h
	g++ -c $< $(CPPFLAGS) -o $@

functions.o: functions.cpp functions.h io.h
	g++ -c functions.cpp $(CPPFLAGS) -o functions.o

io.o: io.cpp io.h
	g++ -c io.cpp $(CPPFLAGS) -o io.o

array_io.o: list_io.cpp list_io.h io.h
	g++ -c list_io.cpp $(CPPFLAGS) -o list_io.o

clean:
	rm -f *.o *.out my_results
