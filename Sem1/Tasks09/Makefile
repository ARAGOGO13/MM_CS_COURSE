OPTS = -fstack-protector-all -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=2 -Wwrite-strings -Wcast-align -Wno-long-long -std=gnu99 -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wold-style-definition -Wdeclaration-after-statement -Wbad-function-cast -Wnested-externs -Wuninitialized


all: a01.out a02.out a03.out a04.out a05.out a06.out a07.out a08.out a09.out a10.out

a01.out: main_01.o solve.o test.o
	gcc main_01.o test.o solve.o -o a01.out

main_01.o: main_01.c solve.h
	gcc -c main_01.c $(OPTS) -o main_01.o

a02.out: main_02.o solve.o test.o
	gcc main_02.o test.o solve.o -o a02.out

main_02.o: main_02.c solve.h
	gcc -c main_02.c $(OPTS) -o main_02.o

a03.out: main_03.o solve.o test.o
	gcc main_03.o test.o solve.o -o a03.out

main_03.o: main_03.c solve.h
	gcc -c main_03.c $(OPTS) -o main_03.o

a04.out: main_04.o solve.o test.o
	gcc main_04.o test.o solve.o -o a04.out

main_04.o: main_04.c solve.h
	gcc -c main_04.c $(OPTS) -o main_04.o

a05.out: main_05.o solve.o test.o
	gcc main_05.o test.o solve.o -o a05.out

main_05.o: main_05.c solve.h
	gcc -c main_05.c $(OPTS) -o main_05.o

a06.out: main_06.o solve.o test.o
	gcc main_06.o test.o solve.o -o a06.out

main_06.o: main_06.c solve.h
	gcc -c main_06.c $(OPTS) -o main_06.o

a07.out: main_07.o solve.o test.o
	gcc main_07.o test.o solve.o -o a07.out

main_07.o: main_07.c solve.h
	gcc -c main_07.c $(OPTS) -o main_07.o

a08.out: main_08.o solve.o test.o
	gcc main_08.o test.o solve.o -o a08.out

main_08.o: main_08.c solve.h
	gcc -c main_08.c $(OPTS) -o main_08.o

a09.out: main_09.o solve.o test.o
	gcc main_09.o test.o solve.o -o a09.out

main_09.o: main_09.c solve.h
	gcc -c main_09.c $(OPTS) -o main_09.o

a10.out: main_10.o solve.o test.o
	gcc main_10.o test.o solve.o -o a10.out

main_10.o: main_10.c solve.h
	gcc -c main_10.c $(OPTS) -o main_10.o

solve.o: solve.c solve.h
	gcc -c solve.c $(OPTS) -o solve.o

test.o: test.c test.h
	gcc -c test.c $(OPTS) -o test.o
