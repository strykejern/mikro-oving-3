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

void LEDS ( const int enable )
{
	int bytes_written;
	
	bytes_written = write( driver, &enable, sizeof(enable) );
}

char BUTTONS ()
{
	char buttons;
	
	int bytes_read;
	
	bytes_read = read( driver, &buttons, sizeof(buttons) );
	
	return buttons;
}
