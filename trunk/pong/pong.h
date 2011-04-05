#ifndef PONG_HEADER_INCLUDED
#define PONG_HEADER_INCLUDED

#include <stdbool.h>			//Booleans
 
#define PADDLE_WIDTH 15
#define PADDLE_HEIGHT 50

#define BALL_SIZE 10

#define FRAMES_PER_SECOND 30
#define SLEEP_PER_FRAME 1000000 / FRAMES_PER_SECOND	


typedef struct paddle_s
{
	short xPos;
	short yPos;
	short oldY;
	COLOR c;
	char score;
} paddle_t;

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


#endif	/* PONG_HEADER_INCLUDED */
