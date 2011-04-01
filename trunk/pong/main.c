#include <sys/mman.h>			//For memory mapping
#include <fcntl.h>
#include <stdlib.h>


//Program entry
int main()
{
	char *lcd;
	int file;
	int i = 0;

	//Open the LDC driver file in read write mode
	file = open("/dev/fb0", O_RDWR);

	//memory map file to array (4 bytes * 320x240 pixles)
	lcd = (char*) mmap(0, 320*240*4, PROT_WRITE | PROT_READ, MAP_SHARED, file, 0);

//	for( i = 0; i < 320*240*4; i++ ) 
	{
//		lcd[i] = 128;
	}

	while( 1 ) {
	}

	return 0;
}
