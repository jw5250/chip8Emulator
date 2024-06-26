CC=gcc
EXENAME=main
BIGENDIANFLAG=-D BIG_ENDIAN
#Phony is for specifying targets that may not represent an actual file
.PHONY=clean

main: main.o memory.o cpu.o screen.o loader.o timer.o keyboard.o
	$(CC) main.o cpu.o screen.o memory.o loader.o timer.o keyboard.o -o $(EXENAME) -lSDL2 -lSDL2_Mixer

main.o : main.c
	$(CC) -c main.c -o main.o
cpu.o : cpu.c memory.o
	$(CC) -c cpu.c -o cpu.o
memory.o : memory.c
	$(CC) -c memory.c -o memory.o
screen.o : screen.c
	$(CC) -c screen.c -o screen.o 
loader.o : loader.c
	$(CC) -c loader.c -o loader.o $(BIGENDIANFLAG)
keyboard.o : keyboard.c
	$(CC) -c keyboard.c -o keyboard.o
timer.o : timer.c
	$(CC) -c timer.c -o timer.o


clean :
	rm screen.o
	rm main.o
	rm cpu.o
	rm memory.o
	rm loader.o
	rm timer.o
	rm keyboard.o
	rm $(EXENAME)
