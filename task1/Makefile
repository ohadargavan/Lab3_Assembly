# default goal
all: task1

#linking the object files into one file
task1: start.o main.o util.o
	ld -m elf_i386 start.o main.o util.o -o task1

#compiling the assembly file
start.o: start.s
	nasm -f elf32 start.s -o start.o

#compiling main 
main.o: main.s
	nasm -f elf32 main.s -o main.o

#compiling util 
util.o: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o

#cleaning the created files
clean:
	rm -f *.o task1