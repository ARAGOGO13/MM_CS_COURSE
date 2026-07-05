OPTS = -mfpmath=sse -fstack-protector-all -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -std=gnu99 -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wold-style-definition -Wdeclaration-after-statement -Wbad-function-cast -Wnested-externs -Wmaybe-uninitialized
all: a01.out a02.out a03.out a04.out a05.out a06.out a07.out a08.out a09.out a10.out

a01.out: a01.o solve.o
	gcc a01.o solve.o -lm -o a01.out

a01.o: main_01.c array_io.h array_io.c solve.h
	gcc -c  main_01.c $(OPTS) -o a01.o

a02.out: a02.o solve.o
	gcc a02.o solve.o -lm -o a02.out

a02.o: main_02.c array_io.h array_io.c solve.h
	gcc -c  main_02.c $(OPTS) -o a02.o

a03.out: a03.o solve.o
	gcc a03.o solve.o -lm -o a03.out

a03.o: main_03.c array_io.h array_io.c solve.h
	gcc -c  main_03.c $(OPTS) -o a03.o

a04.out: a04.o solve.o
	gcc a04.o solve.o -lm -o a04.out

a04.o: main_04.c array_io.h array_io.c solve.h
	gcc -c  main_04.c $(OPTS) -o a04.o

a05.out: a05.o solve.o
	gcc a05.o solve.o -lm -o a05.out

a05.o: main_05.c array_io.h array_io.c solve.h
	gcc -c  main_05.c $(OPTS) -o a05.o

a06.out: a06.o solve.o
	gcc a06.o solve.o -lm -o a06.out

a06.o: main_06.c array_io.h array_io.c solve.h
	gcc -c  main_06.c $(OPTS) -o a06.o

a07.out: a07.o solve.o
	gcc a07.o solve.o -lm -o a07.out

a07.o: main_07.c array_io.h array_io.c solve.h
	gcc -c  main_07.c $(OPTS) -o a07.o

a08.out: a08.o solve.o
	gcc a08.o solve.o -lm -o a08.out

a08.o: main_08.c array_io.h array_io.c solve.h
	gcc -c  main_08.c $(OPTS) -o a08.o

a09.out: a09.o solve.o
	gcc a09.o solve.o -lm -o a09.out

a09.o: main_09.c array_io.h array_io.c solve.h
	gcc -c  main_09.c $(OPTS) -o a09.o

a10.out: a10.o solve.o
	gcc a10.o solve.o -lm -o a10.out

a10.o: main_10.c array_io.h array_io.c solve.h
	gcc -c  main_10.c $(OPTS) -o a10.o

solve.o: solve.c solve.h
	gcc solve.c -c $(OPTS) -o solve.o

clean:
	rm -f *.out
