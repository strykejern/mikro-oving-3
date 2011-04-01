#include <sys/mman.h>			//For memory mapping
#include <fcntl.h>			//for open()
#include <stdlib.h>
#include <stdio.h>			//for printf()
#include <string.h>			//for memcpy()

static char *lcd;
static char buffer[307200];

#define PADDLE_WIDTH 15
#define PADDLE_HEIGHT 50


void draw_one_pixel(short x, short y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	int index = (x * 4) + (y * 1280);
	lcd[index + 0] = a;			//A
	lcd[index + 1] = r;			//R
	lcd[index + 2] = g;			//G
	lcd[index + 3] = b;			//B
}

void draw_paddle( short x, short y )
{
	int i, j;
	for( i = x; i < x+PADDLE_WIDTH; i++ ) 
	{
		for( j = y; j < y+PADDLE_HEIGHT; j++ )
		{
			draw_one_pixel( i, j, 255, 255, 255, 0 );
		}
	}
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

	printf( "version 8\n" );

	draw_paddle( 0, 120-(PADDLE_HEIGHT/2) );

	draw_paddle( 320-PADDLE_WIDTH, 120-(PADDLE_HEIGHT/2) );


//	memmove( lcd, &buffer, sizeof(buffer) );
//	for( i = 0; i < 320*240*4; i++ ) {
//		lcd[i] = buffer[i];
//	}

	return 0;
}


