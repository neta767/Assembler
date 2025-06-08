# Compiler and flags
CC = gcc
CFLAGS = -Wall -ansi -pedantic

# Executable target
assembler: main.o pre_proc.o macro_list.o first_pass.o second_pass.o symbols_list.o validations.o util.o machine_code.o code_list.o data_list.o const.o
	$(CC) $(CFLAGS) $^ -o assembler

# Object file rules
# General rule for compiling object files
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

# Specific rules for individual files if needed
main.o: main.c validations.h util.h macro_list.h symbols_list.h machine_code.h const.h code_list.h data_list.h
pre_proc.o: pre_proc.c pre_proc.h validations.h util.h macro_list.h const.h  code_list.h data_list.h
macro_list.o: macro_list.c macro_list.h const.h
first_pass.o: first_pass.c first_pass.h validations.h macro_list.h symbols_list.h util.h const.h  code_list.h data_list.h
second_pass.o: second_pass.c second_pass.h validations.h const.h
symbols_list.o: symbols_list.c symbols_list.h const.h
validations.o: validations.c validations.h util.h macro_list.h symbols_list.h machine_code.h const.h
util.o: util.c util.h macro_list.h symbols_list.h const.h
machine_code.o: machine_code.c machine_code.h validations.h symbols_list.h macro_list.h util.h const.h code_list.h data_list.h
code_list.o: code_list.c code_list.h const.h
data_list.o: data_list.c data_list.h const.h
const.o: const.c const.h

# Clean up object files and the executable
clean:
	rm -f *.o assembler *.am *.ob *.ent *.ext
