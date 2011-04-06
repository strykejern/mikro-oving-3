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
static bool 	   quit_requested = false;
static GAME_STATE  game_state = STATE_INTRO_SCREEN;

//Public variables
paddle_t player1;
paddle_t player2;
ball_t ballList[MAX_BALLS];

//Private functions
static void read_input();


//Program entry
int main()
{	
	int i, input;

	//Initialize the drivers
	initialize_driver();

	//Initialize random number generator
	srand( time(NULL) );

	//Initialize video
	initialize_video(320, 240, 32);

	//Initialize the sound driver
	initialize_sound_driver();

	//Play menu music
	play_music();

	//Main game loop
	i = 0;
	while( !quit_requested )
	{
		//Count number of frames
		i++;

		switch( game_state )
		{
			//Intro screen
			case STATE_INTRO_SCREEN:
				render_intro_screen();
				input = BUTTONS();
				//Button 4 is exit game, all the others are start
				if(input & 8) quit_requested = true;
				else if(input) game_state = STATE_NEW_GAME;
			break;

			//Fall through state to start new game
			case STATE_NEW_GAME:
				//Initialize the players
				reset_players();
			
				//Initialize the balls
				for( i = 0; i < MAX_BALLS; i++ )
				{
					reset_ball( &ballList[i] );
					ballList[i].enabled = false;
				}
				ballList[0].enabled = true;

				//Let it go!
				game_state = STATE_PLAYING;
			break;

			//playing some pong!
			case STATE_PLAYING:
				//Read input
				read_input();
		
				//Make stuff move and collide
				update_physics();
		
				//Draw this frame
				render_screen();
			
				//Add a new ball every now and then (max ten balls)
				if( i >= 7*FRAMES_PER_SECOND )
				{
					for( i = 0; i < MAX_BALLS; i++ )
					{
						if( !ballList[i].enabled )
						{
							ballList[i].enabled = true;
							break;
						}
					}
					i = 0;	//reset counter
				}
	
				//See if a player has won
				if( player1.score >= 4 || player2.score >= 4 )
				{
					game_state = STATE_WINNING_SCREEN;
					i = 0;	//reset counter;
				}
			break;
	
			//Win screen
			case STATE_WINNING_SCREEN:
				render_winning_screen( player1.score >= 4 ? 1 : 2 );
				//wait a few frames before allowing to quit winning screen
				if( i >= 60 && BUTTONS() ) game_state = STATE_INTRO_SCREEN;
			break;
		}

		//Take it easy, relax a bit
		usleep(SLEEP_PER_FRAME);	//30 frames per second
	}

	//Clear screen on exit
	clear_screen();

	//now quit!
	exit(EXIT_SUCCESS);
}


//Read and handle any input from the buttons
void read_input()
{
	int input = BUTTONS();

	//Exit game button
	if( 8 == (input & 8) ) game_state = STATE_INTRO_SCREEN;	//quit

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
	player1.c.r = 0;
	player1.c.g = rand() % 255;
	player1.c.b = 255;
	player1.score = 0;
	
	//Player 2
	player2.xPos = 320-PADDLE_WIDTH;
	player2.yPos = 120-(PADDLE_HEIGHT/2);
	player2.c.r = 255;
	player2.c.g = rand() % 255;
	player2.c.b = 0;
	player2.score = 0;

	//Reset score display
	LED_update_score();
}

void remove_ball( ball_t *whichBall )
{
	int i;

	//A ball hit the edges, remove it from game
	whichBall->enabled = false;
	reset_ball(whichBall);
	LED_update_score();

	//We need at least one ball to play!
	for( i = 0; i < MAX_BALLS; i++ )
	{
		if( ballList[i].enabled ) break;
	}
	if( i == MAX_BALLS ) ballList[0].enabled = true;
}
