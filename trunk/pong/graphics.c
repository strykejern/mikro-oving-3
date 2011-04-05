#include <math.h>	//for circle drawing

#include "pong.h"
#include "graphics.h"

const bool BallBitmap[10][10] = {
	{0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
	{0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
	{0, 0, 0, 0, 1, 1, 0, 0, 0, 0}
};

const bool PlayerBitmap[5][27] = {
	{1,1,1,0,0,1,0,0,0,0,1,1,0,0,1,0,1,0,1,1,1,1,0,1,1,1,0,},
	{1,0,0,1,0,1,0,0,0,1,0,0,1,0,1,0,1,0,1,0,0,0,0,1,0,0,1,},
	{1,1,1,0,0,1,0,0,0,1,1,1,1,0,0,1,0,0,1,1,1,0,0,1,1,1,0,},
	{1,0,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,1,0,1,0,},
	{1,0,0,0,0,1,1,1,0,1,0,0,1,0,0,1,0,0,1,1,1,1,0,1,0,0,1,}
};

const bool NumberBitmap[50][4] = {
  {0, 1, 1 ,0},
  {1, 0, 0, 1},
  {1, 0, 0, 1},
  {1, 0, 0, 1},
  {0, 1, 1, 0},

  {0, 0, 1 ,0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 0},

  {0, 1, 1 ,0},
  {1, 0, 0, 1},
  {0, 0, 1, 0},
  {0, 1, 0, 0},
  {1, 1, 1, 1},

  {0, 1, 1 ,0},
  {0, 0, 0, 1},
  {0, 0, 1, 0},
  {0, 0, 0, 1},
  {0, 1, 1, 0},

  {0, 1, 0 ,1},
  {1, 0, 0, 1},
  {1, 1, 1, 1},
  {0, 0, 0, 1},
  {0, 0, 0, 1},

  {1, 1, 1 ,1},
  {1, 0, 0, 0},
  {1, 1, 1, 0},
  {0, 0, 0, 1},
  {1, 1, 1, 0},

  {0, 1, 1 ,0},
  {1, 0, 0, 0},
  {1, 1, 1, 0},
  {1, 0, 0, 1},
  {0, 1, 1, 0},

  {1, 1, 1 ,1},
  {0, 0, 0, 1},
  {0, 0, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 0},

  {0, 1, 1 ,0},
  {1, 0, 0, 1},
  {0, 1, 1, 0},
  {1, 0, 0, 1},
  {0, 1, 1, 0},

  {0, 1, 1 ,0},
  {1, 0, 0, 1},
  {0, 1, 1, 1},
  {0, 0, 0, 1},
  {1, 1, 1, 0}
};


void draw_number(int x, int y, char num, COLOR c);

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
	int x, y;

	for( x = 0; x < BALL_SIZE; x++ )
	{
		for( y = 0; y < BALL_SIZE; y++ )
		{
			if( BallBitmap[x][y] ) 
				draw_one_pixel( whichBall->oldXPos+x-BALL_SIZE/2, whichBall->oldYPos+y-BALL_SIZE/2, COLOR_BLACK );
		}
	}
}

//Draws a single pong ball
void draw_ball( ball_t *whichBall )
{
	int x, y;

	//Clear old ball
	clear_ball(whichBall);

	//Draw new ball
	for( x = 0; x < BALL_SIZE; x++ )
	{
		for( y = 0; y < BALL_SIZE; y++ )
		{
			if( BallBitmap[x][y] ) 
				draw_one_pixel( whichBall->xPos+x-BALL_SIZE/2, whichBall->yPos+y-BALL_SIZE/2, COLOR_WHITE );
		}
	}

	whichBall->oldXPos = whichBall->xPos;
	whichBall->oldYPos = whichBall->yPos;
}

//Draw all the game components
void render_screen()
{
	int i;

	//Draw player names
	draw_number( 0, 0, 1, player1.c );
	draw_number( 0, get_screen_width()-10, 2, player2.c );

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

void draw_number(int x, int y, char num, COLOR c)
{
	int i;
	int j;
	
	for(j = 0; j < 5; j++)
	{
		for(i = 0; i < 4; i++)
		{
			if(NumberBitmap[(num * 5) + j][i])
				draw_one_pixel(x + i, y + j, c);
		}
	}
}
