CPPFLAGS = -std=c++11 -O3 -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format

CPPFLAGS += -DM=10

all: a01.out a02.out a03.out a04.out a05.out a06.out a07.out a08.out a09.out a10.out

a%.out: main_%.o functions.o data.o io.o
	g++ $(CPPFLAGS) $^ -o $@ -lm

main_%.o: main_%.cpp data.h functions.h io.h
	g++ -c $< $(CPPFLAGS) -o $@

functions.o: functions.cpp functions.h data.h
	g++ -c functions.cpp $(CPPFLAGS) -o functions.o

data.o: data.cpp data.h io.h
	g++ -c data.cpp $(CPPFLAGS) -o data.o

io.o: io.cpp io.h data.h
	g++ -c io.cpp $(CPPFLAGS) -o io.o

clean:
	rm -f *.o *.out
