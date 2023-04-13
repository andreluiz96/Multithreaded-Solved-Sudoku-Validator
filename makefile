all: arquivo
arquivo: sudoku.c
	gcc -pthread -o sudoku sudoku.c -lm

