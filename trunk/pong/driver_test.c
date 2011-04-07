#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>


//Program entry
int main()
{
	int led_driver;
	
	int leds;
	
	int bytes_written;
	
	int buttons;
	
	int bytes_read;
	
	leds = 0xF0; // Data to be written to the device
	
	printf( "Opening file...\n" );

        // Opening the device driver file
	led_driver = open("/dev/stk1000_driver", O_RDWR);
		
	if( led_driver != -1 )
	{
		printf( "Writing to file...\n" );
                // Writing data to the device
		bytes_written = write( led_driver, &leds, sizeof(leds) );
		
		printf( "%d bytes written: %d\n", bytes_written, leds );

                // Reading data form the device
		bytes_read = read( led_driver, &buttons, sizeof(buttons) );
		printf( "%d bytes read: %d\n", bytes_read, buttons );
	
		printf( "Closing file...\n" );
                // Closing the device driver file
		close(led_driver);
	}
	else
	{
		printf( "FAIL!\n" );
	}
	
	printf( "Finished.\n" );
	
	return 0;
}
