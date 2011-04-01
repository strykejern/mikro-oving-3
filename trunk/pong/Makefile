#Simple makefile for assignment 2
BINARY = pong.elf

#Compiling phase
compile: 	main.c
		avr32-linux-gcc -Wall -g -c $^
		avr32-linux-gcc -o $(BINARY) main.o -lm

#Cleanup object files and binaries
clean:		
		rm -rf *.o

