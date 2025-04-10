assembler: main.o pre_proc.o util.o file.o pre_proc.o macros.o
	gcc -ansi -Wall -pedantic main.o file.o pre_proc.o macros.o util.o -o assembler

main.o: main.c pre_proc.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o

file.o: file.c file.h util.h
	gcc -c -ansi -Wall -pedantic file.c -o file.o

pre_proc.o: pre_proc.c pre_proc.h file.h macros.h util.h definitions.h
	gcc -c -ansi -Wall -pedantic pre_proc.c -o pre_proc.o

macros.o: macros.c macros.h definitions.h
	gcc -c -ansi -Wall -pedantic macros.c -o macros.o

util.o: util.c util.h macros.h definitions.h
	gcc -c -ansi -Wall -pedantic util.c -o util.o

clean:
	rm -f *.o assembler *.am
