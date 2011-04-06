#ifndef PONG_HEADER_INCLUDED
#define PONG_HEADER_INCLUDED

#include <stdbool.h>			//Booleans
#include "screen.h"			//Color typedef

//Define constants 
#define PADDLE_WIDTH 5
#define PADDLE_HEIGHT 50

#define MAX_BALLS 10

#define BALL_SIZE 10

#define FRAMES_PER_SECOND 30
#define SLEEP_PER_FRAME 1000000 / FRAMES_PER_SECOND	

//Define data structures
typedef struct paddle_s
{
	short xPos;
	short yPos;
	short oldY;
	COLOR c;
	char score;
} paddle_t;

//Ball structure
typedef struct ball_s
{
	bool  enabled;
	short oldXPos;
	short oldYPos;
	short xPos;
	short yPos;
	short xSpeed;
	short ySpeed;
} ball_t;

//Game states
typedef enum gamestate_e
{
	STATE_INTRO_SCREEN,
	STATE_NEW_GAME,
	STATE_PLAYING,
	STATE_WINNING_SCREEN

} GAME_STATE;


//Global variables
extern ball_t ballList[MAX_BALLS];
extern paddle_t player1;
extern paddle_t player2;

//Public functions
void reset_ball( ball_t *whichBall );
void remove_ball( ball_t *whichBall );
void reset_players();
void LED_update_score();

#endif	/* PONG_HEADER_INCLUDED */
