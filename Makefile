CC=gcc
EXENAME=main
main: main.o memory.o cpu.o screen.o
	$(CC) main.o cpu.o screen.o memory.o -o $(EXENAME) -lSDL2

main.o : main.c
	$(CC) -c main.c -o main.o
cpu.o : cpu.c memory.o
	$(CC) -c cpu.c -o cpu.o
memory.o : memory.c
	$(CC) -c memory.c -o memory.o
screen.o : screen.c
	$(CC) -c screen.c -o screen.o 

clean :
	rm screen.o
	rm main.o
	rm cpu.o
	rm memory.o
	rm $(EXENAME)