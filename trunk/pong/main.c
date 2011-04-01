#include <sys/mman.h>			//For memory mapping
#include <fcntl.h>			//for open()
#include <stdlib.h>
#include <stdio.h>			//for printf()
#include <string.h>			//for memcpy()

static char *lcd;
static char buffer[307200];

//HERER

void draw_one_pixel(short x, short y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	int index = (x * 4) + (y * 1280);
	lcd[index + 0] = 0;			//A
	lcd[index + 1] = 250;			//R
	lcd[index + 2] = 0;			//G
	lcd[index + 3] = 0;			//B
}

//Program entry
int main()
{	
	int file;
	int x, y;

	//Open the LDC driver file in read write mode
	file = open("/dev/fb0", O_RDWR);

	//memory map file to array (4 bytes * 320x240 pixles)
	lcd = (char*) mmap(0, 320*240*4, PROT_WRITE | PROT_READ, MAP_SHARED, file, 0);

	//Clear screen
	memset( lcd, 0, 320*240*4 );

	printf( "version 6\n" );

	//Fill screen with red pixels
	for( x = 0; x < 320; x++ ) 
	{
		for( y = 0; y < 240; y++ )
		{
			draw_one_pixel(x, y, 255, 0, 0, 0);
		}
	}
//	memmove( lcd, &buffer, sizeof(buffer) );
//	for( i = 0; i < 320*240*4; i++ ) {
//		lcd[i] = buffer[i];
//	}

	return 0;
}



