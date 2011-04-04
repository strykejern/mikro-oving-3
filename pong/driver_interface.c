#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "driver_interface.h"

static int driver;

void initialize_driver()
{
	driver = open("/dev/stk1000_driver", O_RDWR);
	
	if (driver == -1) printf("DRIVER FAIL!");
}

void LEDS ( int enable )
{
	int leds = enable;
	
	int bytes_written;
	
	bytes_written = write( driver, &leds, sizeof(leds) );
	
	leds = 0x00;
}

int BUTTONS ()
{
	int buttons;
	
	int bytes_read;
	
	bytes_read = read( driver, &buttons, sizeof(buttons) );
	
	return buttons;
}
