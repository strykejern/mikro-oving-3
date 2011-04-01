#include <sys/mman.h>			//For memory mapping
#include <fcntl.h>			//for open()
#include <stdlib.h>
#include <stdio.h>			//for printf()
#include <string.h>			//for memcpy()
#include <time.h>


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
	short oldXPos;
	short oldYPos;
	short xPos;
	short yPos;
	short xSpeed;
	short ySpeed;
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
	lcd[index + 1] = b;			//R
	lcd[index + 2] = g;			//G
	lcd[index + 3] = r;			//B
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
	int i, j;

	//Clear old ball
	for( i = theBall.oldXPos; i < theBall.oldXPos+10; i++ ) 
	{
		for( j = theBall.oldYPos; j < theBall.oldYPos+10; j++ )
		{
			draw_one_pixel( i, j, 0, 0, 0, 0 );
		}
	}

	//Draw new ball
	for( i = theBall.xPos; i < theBall.xPos+10; i++ ) 
	{
		for( j = theBall.yPos; j < theBall.yPos+10; j++ )
		{
			draw_one_pixel( i, j, 255, 255, 255, 0 );
		}
	}

	theBall.oldXPos = theBall.xPos;
	theBall.oldYPos = theBall.yPos;
}

void flip_buffers()
{
	int i;
	for( i = 0; i < 320*240*4; i++ ) {
		lcd[i] = buffer[i];
	}
}

//Program entry
int main()
{	
	int file;
	long delay;

	//Open the LDC driver file in read write mode
	file = open("/dev/fb0", O_RDWR);

	//memory map file to array (4 bytes * 320x240 pixles)
	lcd = (char*) mmap(0, 320*240*4, PROT_WRITE | PROT_READ, MAP_SHARED, file, 0);

	//Clear screen
	memset( lcd, 0, 320*240*4 );

	printf( "version 10\n" );


	//Initialize the players
	player1.xPos = 0;
	player1.yPos = 120-(PADDLE_HEIGHT/2);
	player1.r = 255;
	player1.g = 0;
	player1.b = 0;

	player2.xPos = 320-PADDLE_WIDTH;
	player2.yPos = 120-(PADDLE_HEIGHT/2);
	player2.r = 255;
	player2.g = 255;
	player2.b = 0;

	//Initialize the ball
	theBall.oldXPos = theBall.xPos = 160;
	theBall.oldYPos = theBall.yPos = 120;
	theBall.xSpeed = -1;
	theBall.ySpeed = -1;


	//Main game loop
	while( 1 )
	{
		theBall.xPos += theBall.xSpeed;
		theBall.yPos += theBall.ySpeed;

		//Collide with top and bottom
		if( theBall.yPos <= 0 || theBall.yPos+10 >= 240 )
		{
			theBall.ySpeed = -theBall.ySpeed;
		}

		//Collide with left and right
		if( theBall.xPos <= 0 || theBall.xPos+10 >= 320 )
		{
			theBall.xSpeed = -theBall.xSpeed;
		}

		draw_paddle( player1 );
		draw_paddle( player2 );	
		draw_ball();

		//Busy wait
		while(delay > 0) delay--;
		delay = 250000;

		printf("Finished frame\n");
	}


//	memmove( lcd, &buffer, sizeof(buffer) );
//	for( i = 0; i < 320*240*4; i++ ) {
//		lcd[i] = buffer[i];
//	}

	return 0;
}

