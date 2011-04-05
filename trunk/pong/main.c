#include <stdlib.h>
#include <stdio.h>			//for printf()
#include <unistd.h>			//For sleep
#include <time.h>			//For random seed

#include "graphics.h"
#include "pong.h"
#include "driver_interface.h"
#include "sound.h"

#define MAX_BALLS 10

//Private variables
static paddle_t player1;
static paddle_t player2;
static ball_t ballList[MAX_BALLS];
static bool game_active = true;

//Private functions
void render_screen();
void draw_paddle( paddle_t *whichPaddle );
void draw_ball( ball_t *whichBall );
void clear_ball( ball_t *whichBall );

bool paddle_collides( paddle_t *whichPaddle, ball_t *whichBall );
void reset_ball( ball_t *whichBall );
void read_input();
void initialize_players();
void update_physics();
void LED_update_score();

//Program entry
int main()
{	
	int i;

	//Initialize the drivers
	initialize_driver();

	//Initialize random number generator
	srand( time(NULL) );

	//Initialize video
	initialize_video(320, 240, 32);

	//Initialize the sound driver
	initialize_sound_driver();
	play_music();

	//Initialize the players
	initialize_players();

	//Reset score display
	LED_update_score();

	//Initialize the balls
	for( i = 0; i < 10; i++ )
	{
		reset_ball( &ballList[i] );
		ballList[i].enabled = true;
	}

	//Main game loop
	while( game_active )
	{
		//Read input
		read_input();

		//Make stuff move and collide
		update_physics();

		//Draw this frame
		render_screen();

		//Take it easy, relax a bit
		usleep(SLEEP_PER_FRAME);	//30 frames per second
	}

	//Clear screen on exit
	clear_screen();

	return EXIT_SUCCESS;
}

//Draw one paddle
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

//This clears the pong ball
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

//Draws a single pong ball
void draw_ball( ball_t *whichBall )
{
	int i, j;

	//Clear old ball
	clear_ball( whichBall );

	//Draw new ball
	for( i = whichBall->xPos; i < whichBall->xPos+BALL_SIZE; i++ ) 
	{
		for( j = whichBall->yPos; j < whichBall->yPos+BALL_SIZE; j++ )
		{
			draw_one_pixel( i, j, COLOR_WHITE );
		}
	}

	whichBall->oldXPos = whichBall->xPos;
	whichBall->oldYPos = whichBall->yPos;
}

//Returns true if the specified paddle collides with the specified ball
bool paddle_collides( paddle_t *whichPaddle, ball_t *whichBall )
{
	if( whichBall->xPos+BALL_SIZE >= whichPaddle->xPos 
	 && whichBall->yPos+BALL_SIZE >= whichPaddle->yPos
	 && whichBall->xPos <= whichPaddle->xPos + PADDLE_WIDTH 
	 && whichBall->yPos <= whichPaddle->yPos + PADDLE_HEIGHT ) return true;
	return false;
}

//Returns true if the specified ball collides with the specified ball
bool ball_collides( ball_t *firstBall, ball_t *secondBall )
{
	if( firstBall->xPos+BALL_SIZE >= secondBall->xPos 
	 && firstBall->yPos+BALL_SIZE >= secondBall->yPos
	 && firstBall->xPos <= secondBall->xPos + BALL_SIZE 
	 && firstBall->yPos <= secondBall->yPos + BALL_SIZE ) return true;
	return false;
}

//Resets the ball to it's starting position
void reset_ball( ball_t *whichBall )
{
	short xSpeed, ySpeed;

	//Remove old ball first
	clear_ball( whichBall );

	//Middle of the screen
	whichBall->oldXPos = whichBall->xPos = 160;
	whichBall->oldYPos = whichBall->yPos = 120;

	//Randomize ball velocity
	xSpeed = 1 + rand() % 3;
	ySpeed = 1 + rand() % 3;
	if( rand() & 1 ) xSpeed = -xSpeed;
	if( rand() & 1 ) ySpeed = -ySpeed;
	whichBall->xSpeed = xSpeed;
	whichBall->ySpeed = ySpeed;
}

//Read and handle any input from the buttons
void read_input()
{
	int input = BUTTONS();

	//Exit game button
	if( 8 == (input & 8) ) game_active = 0;		//quit

	//Player 2 Controls
	if( 1 == (input & 1) ) 				//player 2 up
	{
		player2.yPos-=7;		
		if( player2.yPos < 0 ) player2.yPos = 0;
	}
	else if( 2 == (input & 2) )			//player 2 down
	{
		player2.yPos+=7;	
		if( player2.yPos+PADDLE_HEIGHT > 239 ) player2.yPos = 240-PADDLE_HEIGHT-1;
	}

	//Player 1 Controls
	if( 128 == (input & 128) ) 			//player 1 up
	{
		player1.yPos-=7;	
		if( player1.yPos < 0 ) player1.yPos = 0;
	}
	else if( 64 == (input & 64) ) 			//player 1 down
	{
		player1.yPos+=7;	
		if( player1.yPos+PADDLE_HEIGHT > 239 ) player1.yPos = 240-PADDLE_HEIGHT-1;
	}
}

//Change the LED lights according to player scores
void LED_update_score()
{
	unsigned char leds = 0;
	leds |= 1*(player2.score >= 1); 
	leds |= 2*(player2.score >= 2); 
	leds |= 4*(player2.score >= 3); 

	leds |= 128*(player1.score >= 1); 
	leds |= 64*(player1.score >= 2); 
	leds |= 32*(player1.score >= 3); 

	LEDS(leds);
}

void do_ball_collision( ball_t * whichBall )
{
	int i = 0;

	//Move the ball
	whichBall->xPos += whichBall->xSpeed;
	whichBall->yPos += whichBall->ySpeed;

	//Collide with top and bottom
	if( whichBall->yPos <= 0 )
	{
		whichBall->yPos = 0;
		whichBall->ySpeed = -whichBall->ySpeed;
	}
	else if( whichBall->yPos+BALL_SIZE >= 240 )
	{
		whichBall->yPos = 240-BALL_SIZE;
		whichBall->ySpeed = -whichBall->ySpeed;
	}

	//Collide with left and right
	if( whichBall->xPos <= 0 )
	{
		//player 1 loses
		player2.score++;
		reset_ball(whichBall);
		LED_update_score();
	}
	else if( whichBall->xPos+BALL_SIZE >= 320 )
	{
		//player 2 loses
		player1.score++;
		reset_ball(whichBall);
		LED_update_score();
	}

	//Collide with paddles
	if( paddle_collides(&player1, whichBall) )
	{
		whichBall->xSpeed = -whichBall->xSpeed + 1;
		whichBall->xPos += whichBall->xSpeed;
	}
	else if( paddle_collides(&player2, whichBall) )
	{
		whichBall->xSpeed = -whichBall->xSpeed - 1;
		whichBall->xPos += whichBall->xSpeed;
	}

	//Collide with all other balls!
	for( i = 0; i < MAX_BALLS; i++ )
	{
		//Was this the last ball?
		if( !ballList[i].enabled ) break;

		//Dont collide with ourselves!
		if( &ballList[i] == whichBall ) continue;

		//Handle collisions
		if( ball_collides(whichBall, &ballList[i]) )
		{
			whichBall->xSpeed = -whichBall->xSpeed;
			whichBall->ySpeed = -whichBall->ySpeed;
			ballList[i].xSpeed = -ballList[i].xSpeed;
			ballList[i].ySpeed = -ballList[i].ySpeed;
		}
	}
}

//Do collisions between ball, wall and paddles
void update_physics()
{
	int i;

	//All active balls
	for( i = 0; i < MAX_BALLS; i++ )
	{
		if( !ballList[i].enabled ) break;
		do_ball_collision( &ballList[i] );
	}	
}

//Draw all the game components
void render_screen()
{
	int i;

	//Draw both paddles
	draw_paddle( &player1 );
	draw_paddle( &player2 );

	//Draw all active balls
	for( i = 0; i < MAX_BALLS; i++ )
	{
		if( !ballList[i].enabled ) break;
		draw_ball( &ballList[i] );
	}

	//Show the result on the LCD
	flip_buffers();
}

//Reset players and their positions
void initialize_players()
{
	player1.xPos = 0;
	player1.yPos = 120-(PADDLE_HEIGHT/2);
	player1.c.r = 255;
	player1.c.g = 0;
	player1.c.b = 0;
	player1.score = 0;
	
	player2.xPos = 320-PADDLE_WIDTH;
	player2.yPos = 120-(PADDLE_HEIGHT/2);
	player2.c.r = 255;
	player2.c.g = 255;
	player2.c.b = 0;
	player2.score = 0;
}
