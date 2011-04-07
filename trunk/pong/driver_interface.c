#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "driver_interface.h"

static int driver;

void initialize_driver()
{
        // Opening the driver file
	driver = open("/dev/stk1000_driver", O_RDWR);
	
	if (driver == -1) printf("DRIVER FAIL!");
}

void LEDS ( int enable )
{
	int leds = enable;
	
	int bytes_written;

        // Writing data form enable to the driver
	bytes_written = write( driver, &leds, sizeof(leds) );
}

int BUTTONS ()
{
	int buttons;
	
	int bytes_read;

        // Reads button data from the driver
	bytes_read = read( driver, &buttons, sizeof(buttons) );
	
	return buttons;
}


