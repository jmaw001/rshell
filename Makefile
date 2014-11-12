#compiler
CC=g++

#flags

Flags= -Wall -Werror -ansi -pedantic

all: rshell cp ls

rshell:

	mkdir -p bin
	$(CC) $(Flags) src/rshell.cpp -o bin/rshell

ls:

	mkdir -p bin
	$(CC) $(Flags) src/ls.cpp -o bin/ls

cp:
	mkdir-p bin
	$(cc) $(Flags) src/cp.cpp -o bin/cp
