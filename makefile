CXX		:=g++
CFLAGS		:=-g -Wall
EXECUTABLE	:=yace

all : main.o Chip8.o CPU.o
	$(CXX) $(CFLAGS) -o $(EXECUTABLE) main.o Chip8.o CPU.o

main.o : main.cpp
	$(CXX) $(CFLAGS) -c main.cpp

Chip8.o : src/Chip8.cpp include/Chip8.h
	$(CXX) $(CFLAGS) -c src/Chip8.cpp

CPU.o : src/CPU.cpp include/CPU.h
	$(CXX) $(CFLAGS) -D _DEBUG_ -c src/CPU.cpp

.PHONY : clean
clean:
	rm $(EXECUTABLE) main.o Chip8.o CPU.o
