#ifndef PONG_HEADER_INCLUDED
#define PONG_HEADER_INCLUDED
 
#define PADDLE_WIDTH 15
#define PADDLE_HEIGHT 50

#define BALL_SIZE 10

#define FRAMES_PER_SECOND 30
#define SLEEP_PER_FRAME 1000000 / FRAMES_PER_SECOND	


typedef struct color_s
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} COLOR;

typedef struct paddle_s
{
	short xPos;
	short yPos;
	short oldY;
	COLOR c;
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


const static COLOR COLOR_BLACK = {0, 0, 0, 0};
const static COLOR COLOR_WHITE = {255, 255, 255, 0};

#endif	/* PONG_HEADER_INCLUDED */
