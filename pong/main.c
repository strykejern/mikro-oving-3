#include <sys/mman.h>			//For memory mapping
#include <fcntl.h>			//for open()
#include <stdlib.h>
#include <stdio.h>			//for printf()
#include <string.h>			//for memcpy()

static char *lcd;
static char buffer[307200];

typedef unsigned char COLOR;

typedef struct paddle_s
{
	short xPos;
	short yPos;
	COLOR r;
	COLOR g;
	COLOR b;
} paddle_t;

typedef struct ball_s
{
	short xPos;
	short yPos;
} ball_t;

static paddle_t player1;
static paddle_t player2;
static ball_t theBall;

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

void draw_paddle( paddle_t whichPaddle )
{
	int i, j;
	for( i = whichPaddle.xPos; i < whichPaddle.xPos+PADDLE_WIDTH; i++ ) 
	{
		for( j = whichPaddle.yPos; j < whichPaddle.yPos+PADDLE_HEIGHT; j++ )
		{
			draw_one_pixel( i, j, whichPaddle.r, whichPaddle.g, whichPaddle.b, 0 );
		}
	}
}

void draw_ball()
{
	
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


	//Initialize the players
	player1.xPos = 0;
	player1.yPos = 120-(PADDLE_HEIGHT/2);
	player1.r = 255;
	player1.g = 0;
	player1.b = 255;

	player2.xPos = 320-PADDLE_WIDTH;
	player2.yPos = 120-(PADDLE_HEIGHT/2);
	player2.r = 0;
	player2.g = 255;
	player2.b = 255;

	//Initialize the ball
	theBall.xPos = 160;
	theBall.yPos = 120;

	//Draw their paddles
	draw_paddle( player1 );
	draw_paddle( player2 );


//	memmove( lcd, &buffer, sizeof(buffer) );
//	for( i = 0; i < 320*240*4; i++ ) {
//		lcd[i] = buffer[i];
//	}

	return 0;
}


