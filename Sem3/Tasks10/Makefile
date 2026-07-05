CPPFLAGS = -std=c++11 -O3 -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic\
 -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=2 -Wwrite-strings\
  -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-property-attribute-mismatch

all: a.out

a.out: main.o list.o
	g++ $(CPPFLAGS) $^ -o $@ -lm

main.o: main.cpp io.h student.h list.h tree.h
	g++ -c main.cpp $(CPPFLAGS) -o main.o

list.o: list.cpp list.h io.h student.h
	g++ -c list.cpp $(CPPFLAGS) -o list.o

clean:
	rm -f *.o a.out