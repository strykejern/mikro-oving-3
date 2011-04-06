#include <sys/mman.h>			//For memory mapping
#include <fcntl.h>			//for open()
#include <string.h>			//for memcpy()

#include "screen.h"

//Private variables
static char buffer[320*240*4];
static char *lcd;

static int screen_width;
static int screen_height;
static int screen_bytes;
static int bytes_per_pixel;

void draw_one_pixel(const short x, const short y, COLOR color )
{
	//Prevent segfaults when drawing outside the screen
	if( x < 0 || y < 0 || x >= screen_width || y >= screen_width ) return;

	int index = (x * 4) + (y * 1280);
	buffer[index + 0] = color.a;		//A
	buffer[index + 1] = color.b;		//B
	buffer[index + 2] = color.g;		//G
	buffer[index + 3] = color.r;		//R
}

void initialize_video( const int width, const int height, const int depth )
{
	int file;

	screen_width = width;
	screen_height = height;
	bytes_per_pixel = depth >> 3;
	screen_bytes = screen_width*screen_height*bytes_per_pixel;

	//Open the LDC driver file in read/write mode
	file = open("/dev/fb0", O_RDWR);

	//memory map file to array (4 bytes * 320x240 pixles)
	lcd = (char*) mmap(0, screen_bytes, PROT_WRITE | PROT_READ, MAP_SHARED, file, 0);

	//Clear screen
	clear_screen();
}

//Clears the screen to black
void clear_screen()
{
	//Clear the screen with black pixels
	memset( lcd, 0, screen_bytes );

	//Clear the buffer as well
	memset( buffer, 0, screen_bytes );
}

//Swaps between the buffer and the actual LCD screen
//should be called at the end of every frame
void flip_buffers()
{
	//copy from buffer to screen
	memcpy( lcd, buffer, sizeof(buffer) );

	//Clear the old buffer
	memset( buffer, 0, screen_bytes );
}

int get_screen_width()
{
	return screen_width;
}

int get_screen_height()
{
	return screen_height;
}

