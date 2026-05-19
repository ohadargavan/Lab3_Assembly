# default goal
all: task2

#linking the object files into one file
task2: start.o main.o util.o
	ld -m elf_i386 start.o main.o util.o -o task2

#compiling the assembly file (glue code)
start.o: start.s
	nasm -f elf32 start.s -o start.o

#compiling main (Now back in C!)
main.o: main.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector main.c -o main.o

#compiling util 
util.o: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o

#cleaning the created files
clean:
	rm -f *.o task2