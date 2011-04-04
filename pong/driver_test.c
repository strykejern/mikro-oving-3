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
	
	leds = 0xF0;
	
	printf( "Opening file...\n" );
	
	led_driver = open("/dev/stk1000_driver", O_RDWR);
		
	if( led_driver != -1 )
	{
		printf( "Writing to file...\n" );
		bytes_written = write( led_driver, &leds, sizeof(leds) );
		
		printf( "%d bytes written\n", bytes_written );
		
		bytes_read = read( led_driver, &buttons, sizeof(buttons) );
		printf( "%d bytes read: %d", bytes_read, buttons );
	
		printf( "Closing file...\n" );
		close(led_driver);
	}
	else
	{
		printf( "FAIL!\n" );
	}
	
	printf( "Finished.\n" );
	
	return 0;
}
