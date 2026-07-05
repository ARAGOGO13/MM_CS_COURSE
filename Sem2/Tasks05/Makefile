CFLAGS = -fstack-protector-all -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors \
	-Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=2 -Wwrite-strings \
	-Wcast-align -Wno-long-long -std=gnu99 -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations \
	-Wold-style-definition -Wdeclaration-after-statement -Wbad-function-cast -Wnested-externs -Wuninitialized -O3


all: a01.out a02.out a03.out a04.out a05.out a06.out a07.out a08.out a09.out a10.out a11.out

a%.out: main_%.o solve.o array_io.o
	gcc $(CFLAGS) $^ -o $@

main_%.o: main_%.c solve.h array_io.h
	gcc -c $< $(CFLAGS) -o $@

solve.o: solve.c solve.h
	gcc -c solve.c $(CFLAGS) -o solve.o

array_io.o: array_io.c array_io.h
	gcc -c array_io.c $(CFLAGS) -o array_io.o

clean_b:
	rm b.txt
	touch b.txt

clean:
	rm *.o
	rm *.out
