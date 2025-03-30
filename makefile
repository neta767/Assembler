assembler: main.o file.o
	gcc -ansi -Wall -pedantic main.o file.o -o assembler
main.o: main.c file.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o
file.o: file.c file.h
	gcc -c -ansi -Wall -pedantic file.c -o file.o