#Simple makefile for assignment 2
BINARY = pong.elf

#Compiling phase
compile: 	main.c driver_test.c driver_interface.c
		avr32-linux-gcc -Wall -g -c $^
		avr32-linux-gcc -o $(BINARY) main.o driver_interface.o -lm
		avr32-linux-gcc -o driver_test.elf driver_test.o -lm

#Cleanup object files and binaries
clean:		
		rm -rf *.o

