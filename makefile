assembler: main.o file.o pre_proc.o macro.o
	gcc -ansi -Wall -pedantic main.o file.o pre_proc.o macro.o -o assembler

main.o: main.c file.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o

file.o: file.c file.h
	gcc -c -ansi -Wall -pedantic file.c -o file.o

pre_proc.o: pre_proc.c pre_proc.h file.h macro.h
	gcc -c -ansi -Wall -pedantic pre_proc.c -o pre_proc.o

macro.o: macro.c macro.h
	gcc -c -ansi -Wall -pedantic macro.c -o macro.o

clean:
	rm -f *.o assembler *.am