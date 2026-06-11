CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -std=c11
LDFLAGS = -lm

.PHONY: all clean test

all: introsort gen

introsort: introsort.c
	$(CC) $(CFLAGS) -o introsort introsort.c $(LDFLAGS)

gen: gen.c
	$(CC) $(CFLAGS) -o gen gen.c

# Запуск: n=20 случайных чисел, динамический массив (2)
test: introsort gen
	./gen 20 0
	./introsort input.txt 2

clean:
	rm -f introsort gen input.txt
