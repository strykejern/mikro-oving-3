#Simple makefile for assignment 3
BINARY = main.elf


#Upload the bootloader
bootload:
	avr32program halt
	sleep 3
	avr32program program -r -R -f0,8Mb -e u-boot.bin
