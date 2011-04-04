#include <sys/mman.h>			//For memory mapping
#include <fcntl.h>			//for open()
#include <stdlib.h>
#include <stdio.h>			//for printf()
#include <string.h>			//for memcpy()
#include <unistd.h>			//For sleep
#include <time.h>			//For random seed

#include "driver_interface.h"
#include "pong.h"

//Private variables
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
	for( i = theBall.oldXPos; i < theBall.oldXPos+BALL_SIZE; i++ ) 
	{
		for( j = theBall.oldYPos; j < theBall.oldYPos+BALL_SIZE; j++ )
		{
			draw_one_pixel( i, j, COLOR_BLACK);
		}
	}

	//Draw new ball
	for( i = theBall.xPos; i < theBall.xPos+BALL_SIZE; i++ ) 
	{
		for( j = theBall.yPos; j < theBall.yPos+BALL_SIZE; j++ )
		{
			draw_one_pixel( i, j, COLOR_WHITE );
		}
	}

	theBall.oldXPos = theBall.xPos;
	theBall.oldYPos = theBall.yPos;
}

char paddle_collides( paddle_t whichPaddle )
{
	if( theBall.xPos+BALL_SIZE >= whichPaddle.xPos 
	 && theBall.yPos+BALL_SIZE >= whichPaddle.yPos
	 && theBall.xPos <= whichPaddle.xPos + PADDLE_WIDTH 
	 && theBall.yPos <= whichPaddle.yPos + PADDLE_HEIGHT ) return 1;
	return 0;
}

//Program entry
int main()
{	
	int file;
	short xSpeed, ySpeed;
	int count = 0;

	//Initialize the drivers
	initialize_driver();

	//Initialize random number generator
	srand( time(NULL) );

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

	//Randomize ball velocity
	xSpeed = 1 + rand() % 3;
	ySpeed = 1 + rand() % 3;
	if( rand() & 1 ) xSpeed = -xSpeed;
	if( rand() & 1 ) ySpeed = -ySpeed;
	theBall.xSpeed = xSpeed;
//	theBall.ySpeed = ySpeed;


	//Main game loop
	while( 1 )
	{
		if( BUTTONS() ) break;

		theBall.xPos += theBall.xSpeed;
		theBall.yPos += theBall.ySpeed;

		//Collide with top and bottom
		if( theBall.yPos <= 0 )
		{
			theBall.yPos = 0;
			theBall.ySpeed = -theBall.ySpeed;
		}
		else if( theBall.yPos+BALL_SIZE >= 240 )
		{
			theBall.yPos = 240-BALL_SIZE;
			theBall.ySpeed = -theBall.ySpeed;
		}

		//Collide with left and right
		if( theBall.xPos <= 0 )
		{
			theBall.xPos = 0;
			theBall.xSpeed = -theBall.xSpeed;
		}
		else if( theBall.xPos+BALL_SIZE >= 320 )
		{
			theBall.xPos = 320-BALL_SIZE;
			theBall.xSpeed = -theBall.xSpeed;
		}

		//Collide with paddles
		if( paddle_collides(player1) )
		{
			theBall.xSpeed = -theBall.xSpeed;
			theBall.xPos += theBall.xSpeed;
		}
		else if( paddle_collides(player2) )
		{
			theBall.xSpeed = -theBall.xSpeed;
			theBall.xPos += theBall.xSpeed;
		}

		draw_paddle( player1 );
		draw_paddle( player2 );	
		draw_ball();

		//Take it easy, relax a bit
		usleep(SLEEP_PER_FRAME);	//30 frames per second

//		printf("Finished frame %d\n", count);
		count++;
		LEDS(0xFF);
	}

	//Clear screen
	memset( lcd, 0, 320*240*4 );

	return EXIT_SUCCESS;
}

