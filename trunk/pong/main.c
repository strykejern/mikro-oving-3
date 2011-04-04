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


void draw_paddle( paddle_t *whichPaddle )
{
	int i, j;

	//Only draw if we have moved since last frame
	if( whichPaddle->oldY != whichPaddle->yPos )
	{

		//First clear old paddle
		for( i = whichPaddle->xPos; i < whichPaddle->xPos+PADDLE_WIDTH; i++ ) 
		{
			for( j =whichPaddle->oldY; j < whichPaddle->oldY+PADDLE_HEIGHT; j++ )
			{
				draw_one_pixel( i, j, COLOR_BLACK );
			}
		}
		whichPaddle->oldY = whichPaddle->yPos;

		//Then draw the new one
		for( i = whichPaddle->xPos; i < whichPaddle->xPos+PADDLE_WIDTH; i++ ) 
		{
			for( j = whichPaddle->yPos; j < whichPaddle->yPos+PADDLE_HEIGHT; j++ )
			{
				draw_one_pixel( i, j, whichPaddle->c );
			}
		}
	}
}

void clear_ball( ball_t *whichBall )
{
	int i, j;

	for( i = whichBall->oldXPos; i < whichBall->oldXPos+BALL_SIZE; i++ ) 
	{
		for( j = whichBall->oldYPos; j < whichBall->oldYPos+BALL_SIZE; j++ )
		{
			draw_one_pixel( i, j, COLOR_BLACK);
		}
	}
}

void draw_ball()
{
	int i, j;

	//Clear old ball
	clear_ball( &theBall );

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

char paddle_collides( paddle_t *whichPaddle, ball_t *whichBall )
{
	if( whichBall->xPos+BALL_SIZE >= whichPaddle->xPos 
	 && whichBall->yPos+BALL_SIZE >= whichPaddle->yPos
	 && whichBall->xPos <= whichPaddle->xPos + PADDLE_WIDTH 
	 && whichBall->yPos <= whichPaddle->yPos + PADDLE_HEIGHT ) return 1;
	return 0;
}

void reset_ball()
{
	short xSpeed, ySpeed;

	//Remove old ball first
	clear_ball( &theBall );

	//Middle of the screen
	theBall.oldXPos = theBall.xPos = 160;
	theBall.oldYPos = theBall.yPos = 120;

	//Randomize ball velocity
	xSpeed = 1 + rand() % 3;
	ySpeed = 1 + rand() % 3;
	if( rand() & 1 ) xSpeed = -xSpeed;
	if( rand() & 1 ) ySpeed = -ySpeed;
	theBall.xSpeed = xSpeed;
	theBall.ySpeed = ySpeed;
}

//Program entry
int main()
{	
	int file;
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
	reset_ball();


	//Main game loop
	while( 1 )
	{
		//Read input
		int input = BUTTONS();
		if( 8 == (input & 8) ) break;			//quit
		if( 1 == (input & 1) ) player2.yPos-=5;		//player 2 up
		else if( 2 == (input & 2) ) player2.yPos+=5;	//player 2 down
		if( 128 == (input & 128) ) player1.yPos-=5;	//player 1 up
		else if( 64 == (input & 64) ) player1.yPos+=5;	//player 1 down

		if( player1.yPos < 0 ) player1.yPos = 0;
		if( player2.yPos < 0 ) player2.yPos = 0;
		if( player1.yPos+PADDLE_HEIGHT > 240 ) player1.yPos = 240-PADDLE_HEIGHT;
		if( player2.yPos+PADDLE_HEIGHT > 240 ) player2.yPos = 240-PADDLE_HEIGHT;


		//Move the ball
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
			//player 1 loses
			reset_ball();
		}
		else if( theBall.xPos+BALL_SIZE >= 320 )
		{
			//player 2 loses
			reset_ball();
		}

		//Collide with paddles
		if( paddle_collides(&player1, &theBall) )
		{
			theBall.xSpeed = -theBall.xSpeed + 1;
			theBall.xPos += theBall.xSpeed;
		}
		else if( paddle_collides(&player2, &theBall) )
		{
			theBall.xSpeed = -theBall.xSpeed - 1;
			theBall.xPos += theBall.xSpeed;
		}

		draw_paddle( &player1 );
		draw_paddle( &player2 );	
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

