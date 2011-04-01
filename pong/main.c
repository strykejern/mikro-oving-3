#include <sys/mman.h>			//For memory mapping
#include <fcntl.h>
#include <stdlib.h>

static char *lcd;

void draw_one_pixel(short x, short y, unsigned char r, unsigned char g, unsigned char b)
{
	int index = (x * 4) + (y * 1280);
	lcd[index + 0] = r;			//R
	lcd[index + 1] = g;			//G
	lcd[index + 2] = b;			//B
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

	//Fill screen with red pixels
	for( x = 0; x < 320; x++ ) 
	{
		for( y = 0; y < 240; y++ )
		{
			draw_one_pixel(x, y, 250, 0, 1);
		}
	}

	return 0;
}


