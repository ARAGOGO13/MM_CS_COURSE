CFLAGS = -fstack-protector-all -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors \
	-Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=2 -Wwrite-strings \
	-Wcast-align -Wno-long-long -std=gnu99 -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations \
	-Wold-style-definition -Wdeclaration-after-statement -Wbad-function-cast -Wnested-externs -Wuninitialized -O3


all: a01.out a02.out a03.out a04.out a05.out a06.out a08.out a09.out a10.out a11.out a12.out

a%.out: main_%.o solve.o
	gcc $(CFLAGS) $^ -o $@ -lm

main_%.o: main_%.c solve.h
	gcc -c $< $(CFLAGS) -o $@

solve.o: solve.c solve.h
	gcc -c solve.c $(CFLAGS) -o solve.o

clean_b:
	rm b.txt
	touch b.txt

clean:
	rm *.o
	rm *.out
