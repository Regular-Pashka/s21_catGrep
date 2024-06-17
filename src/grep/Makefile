СС = gcc
FLAGS = -Wall -Werror -Wextra 

all: s21_grep

s21_grep: s21_grep.c s21_grep.h
	$(CC) $(FLAGS) s21_grep.c -o s21_grep

shlang:
	clang-format -n *.h *.c

clean:
	rm s21_grep 

rebuild:
	make clean all 
