#include <sys/mman.h>			//For memory mapping
#include <fcntl.h>			//for open()
#include <stdlib.h>
#include <stdio.h>			//for printf()
#include <string.h>			//for memcpy()
#include <unistd.h>			//For sleep

#include "pong.h"


static char *lcd;

static paddle_t player1;
static paddle_t player2;
static ball_t theBall;

void draw_one_pixel(short x, short y, COLOR color )
{
	int index = (x * 4) + (y * 1280);
	lcd[index + 0] = color.a;		//A
	lcd[index + 1] = color.b;		//B
	lcd[index + 2] = color.g;		//G
	lcd[index + 3] = color.r;		//R
}

void draw_paddle( paddle_t whichPaddle )
{
	int i, j;
	
	//Only draw if we have moved since last frame
	if( whichPaddle.oldY != whichPaddle.yPos )
	{
		whichPaddle.oldY = whichPaddle.yPos;
		for( i = whichPaddle.xPos; i < whichPaddle.xPos+PADDLE_WIDTH; i++ ) 
		{
			for( j = whichPaddle.yPos; j < whichPaddle.yPos+PADDLE_HEIGHT; j++ )
			{
				draw_one_pixel( i, j, whichPaddle.c );
			}
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
			draw_one_pixel( i, j, COLOR_BLACK);
		}
	}

	//Draw new ball
	for( i = theBall.xPos; i < theBall.xPos+10; i++ ) 
	{
		for( j = theBall.yPos; j < theBall.yPos+10; j++ )
		{
			draw_one_pixel( i, j, COLOR_WHITE );
		}
	}

	theBall.oldXPos = theBall.xPos;
	theBall.oldYPos = theBall.yPos;
}

//Program entry
int main()
{	
	int file;
//	long delay;

	printf( "version 11\n" );

	//Open the LDC driver file in read write mode
	file = open("/dev/fb0", O_RDWR);

	//memory map file to array (4 bytes * 320x240 pixles)
	lcd = (char*) mmap(0, 320*240*4, PROT_WRITE | PROT_READ, MAP_SHARED, file, 0);

	//Clear screen
	memset( lcd, 0, 320*240*4 );

	//Initialize the players
	player1.xPos = 0;
	player1.yPos = 120-(PADDLE_HEIGHT/2);
	player1.c.r = 255;
	player1.c.g = 0;
	player1.c.b = 0;

	player2.xPos = 320-PADDLE_WIDTH;
	player2.yPos = 120-(PADDLE_HEIGHT/2);
	player2.c.r = 255;
	player2.c.g = 255;
	player2.c.b = 0;

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
		usleep(10000);
//		while(delay > 0) delay--;
//		delay = 250000;

		printf("Finished frame\n");
	}


	return EXIT_SUCCESS;
}

