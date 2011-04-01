
#include <stdio.h>
#include <sys/mman.h>			//For memory mapping
#include <fcntl.h>
#include <stdlib.h>

int left_paddle_pos = 120;
int right_paddle_pos = 120;


//Program entry
int main()
{
	FILE *led_driver;
	
	printf( "Opening file...\n" );
	
	led_driver = fopen("/dev/led_driver", "w");
		
	if( led_driver )
	{
		printf( "Writing to file...\n" );
		fprintf(led_driver, "%c%c", 0xA, 0xA);
	
		printf( "Closing file...\n" );
		fclose(led_driver);
	}
	else
	{
		printf( "FAIL!" );
	}
	
	printf( "Finished.\n" );
/*
	int *lcd;
	int file;
	int i = 0;

	//Open the LDC driver file in read write mode
	file = open("/dev/fb0", O_RDWR);

	//memory map file to array
	lcd = mmap(NULL, 320*240, PROT_WRITE | PROT_READ, MAP_PRIVATE, file, 0);

	for( i = 0; i < 320*100; i++ ) 
	{
		lcd[i] = 0xFF000000;
	}

	while( 1 ) {
	}

*/
	return 0;
}
