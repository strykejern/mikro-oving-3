#include <stdlib.h>			//For various stuff
#include <stdio.h>			//for printf()
#include <unistd.h>			//For sleep
#include <time.h>			//For random seed
#include <pthread.h>

#include "screen.h"
#include "pong.h"
#include "graphics.h"
#include "driver_interface.h"
#include "sound.h"
#include "physics.h"

//Private variables
static bool game_active = true;

//Public variables
paddle_t player1;
paddle_t player2;
ball_t ballList[MAX_BALLS];

//Private functions
static void read_input();

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
	//play_music();

	//Initialize the players
	reset_players();

	//Initialize the balls
	for( i = 0; i < 5; i++ )
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

//Resets the ball to it's starting position
void reset_ball( ball_t *whichBall )
{
	short xSpeed, ySpeed;

	//Remove old ball first
	if( whichBall->xPos != 0 ) clear_ball( whichBall );

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

//Reset players and their positions
void reset_players()
{
	//Player 1
	player1.xPos = 0;
	player1.yPos = 120-(PADDLE_HEIGHT/2);
	player1.c.r = 255;
	player1.c.g = 0;
	player1.c.b = 0;
	player1.score = 0;
	
	//Player 2
	player2.xPos = 320-PADDLE_WIDTH;
	player2.yPos = 120-(PADDLE_HEIGHT/2);
	player2.c.r = 255;
	player2.c.g = 255;
	player2.c.b = 0;
	player2.score = 0;

	//Reset score display
	LED_update_score();
}

