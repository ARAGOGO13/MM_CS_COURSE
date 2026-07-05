CPPFLAGS = -std=c++11 -O3 -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic\
 -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=2 -Wwrite-strings\
  -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-property-attribute-mismatch

all: a.out

a.out: main.o functions.o tree.o
	g++ $(CPPFLAGS) $^ -o $@ -lm

main.o: main.cpp io.h student.h tree.h
	g++ -c main.cpp $(CPPFLAGS) -o main.o

functions.o: functions.cpp tree.h io.h student.h
	g++ -c functions.cpp $(CPPFLAGS) -o functions.o

tree.o: tree.cpp tree.h
	g++ -c tree.cpp $(CPPFLAGS) -o tree.o

clean:
	rm -f *.o a.out my_results
