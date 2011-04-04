#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "driver_interface.h"

static int driver;

void initialize_driver()
{
	driver = open("/dev/stk1000_driver", O_RDWR);
}

void LEDS ( const unsigned char enable )
{
	int bytes_written;
	
	bytes_written = write( led_driver, &leds, sizeof(leds) );
}

char BUTTONS ()
{
	char buttons;
	
	int bytes_read;
	
	bytes_read = read( led_driver, &buttons, sizeof(buttons) );
	
	return buttons;
}