CPPFLAGS = -std=c++14 -O3 -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic\
 -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=2 -Wwrite-strings\
  -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-property-attribute-mismatch

all: a.out

a.out: main.o
	g++ $(CPPFLAGS) $^ -o $@ -lm

main.o: main.cpp io.h student.h avl_tree.h
	g++ -c main.cpp $(CPPFLAGS) -o main.o

clean:
	rm -f *.o a.out