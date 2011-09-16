CC:=g++
CFLAGS:=-g -Wall
EXECUTABLE:=yace

all : main.o Chip8.o CPU.o
	$(CC) $(CFLAGS) -o $(EXECUTABLE) main.o Chip8.o CPU.o

main.o : main.cpp
	$(CC) $(CFLAGS) -c main.cpp

Chip8.o : src\Chip8.cpp include\Chip8.h
	$(CC) $(CFLAGS) -c src\Chip8.cpp

CPU.o : src\CPU.cpp include\CPU.h
	$(CC) $(CFLAGS) -D _DEBUG_ -c src\CPU.cpp

.PHONY : clean
clean:
	rm $(EXECUTABLE) main.o Chip8.o CPU.o
