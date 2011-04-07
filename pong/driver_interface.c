#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "driver_interface.h"

static int driver;

//Initializes the custom drivers
void initialize_driver()
{
        // Opening the driver file
	driver = open("/dev/stk1000_driver", O_RDWR);
	
	if (driver == -1) printf("DRIVER FAIL!");
}

//Sets which LEDS should be lighted
void LEDS ( int enable )
{
	int leds = enable;
	
	int bytes_written;

        // Writing data form enable to the driver
	bytes_written = write( driver, &leds, sizeof(leds) );
}

//Returns the bits for which buttons have been pressed
int BUTTONS ()
{
	int buttons;
	
	int bytes_read;

        // Reads button data from the driver
	bytes_read = read( driver, &buttons, sizeof(buttons) );
	
	return buttons;
}


