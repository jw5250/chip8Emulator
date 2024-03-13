CC=gcc
main: main.o memory.o cpu.o
	$(CC) main.o cpu.o memory.o -o main
main.o : main.c
	$(CC) -c main.c -o main.o
cpu.o : cpu.c memory.o
	$(CC) -c cpu.c -o cpu.o
memory.o : memory.c
	$(CC) -c memory.c -o memory.o
