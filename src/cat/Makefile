СС = gcc
FLAGS = -Wall -Werror -Wextra 

all: s21_cat

s21_cat: s21_cat.c s21_cat.h
	$(CC) $(FLAGS) s21_cat.c -o s21_cat

shlang:
	clang-format -n *.h *.c

clean:
	rm s21_cat 

rebuild:
	make clean all 
