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

const bool NumberBitmap[40][5] = {
  {0, 1, 1 ,1, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {0, 1, 1, 1, 0},

  {0, 0, 1 ,0, 0},
  {0, 1, 0, 0, 0},
  {1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},

  {0, 1, 0 ,0, 0},
  {1, 0, 0, 0, 1},
  {1, 0, 0, 1, 1},
  {1, 0, 1, 0, 1},
  {0, 1, 0, 0, 1},

  {1, 0, 0 ,0, 1},
  {1, 0, 1, 0, 1},
  {1, 0, 1, 0, 1},
  {1, 1, 1, 1, 1},
  {0, 1, 0, 1, 0},

};


void draw_number(int x, int y, int num, COLOR c);

//Draw one paddle
void draw_paddle( paddle_t *whichPaddle )
{
	int i, j;

	//Only draw if we have moved since last frame
	//if( whichPaddle->oldY != whichPaddle->yPos )
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

void draw_player(int x, int y, COLOR c)
{
	int i;
	int j;
		for(j = 0; j < 5; j++)
		{
			for(i = 0; i < 27; i++)
			{
				if(PlayerBitmap[j][i]) draw_one_pixel(x + i, y + j, c);
		}
	}
}

//Draw all the game components
void render_screen()
{
	int i;

	//Draw player names
	draw_player( 10, 10, player1.c );
	draw_number( 40, 10, 1, player1.c );
	draw_player( get_screen_width()-60, 10, player2.c );
	draw_number( get_screen_width()-20, 10, 2, player2.c );

	//Draw both paddles
	draw_paddle( &player1 );
	draw_paddle( &player2 );

	//Draw player 1 score
	draw_number( (get_screen_width()/2)-15, 10, player1.score, player1.c );

	//Draw a line between scores
	for( i = (get_screen_width()/2)-5; i < 10; i++ )
	{
		draw_one_pixel(i, 5, COLOR_WHITE);
	}

	//Draw player 2 score
	draw_number( (get_screen_width()/2)+15, 10, player2.score, player2.c );


	//Draw all active balls
	for( i = 0; i < MAX_BALLS; i++ )
	{
		if( !ballList[i].enabled ) break;
		draw_ball( &ballList[i] );
	}

	//Show the result on the LCD
	flip_buffers();
}

void draw_number(int x, int y, int num, COLOR c)
{
	int i;
	int j;
	
	for(i = 0; i < 5; i++)
	{
		for(j = 0; j < 5; j++)
		{
			if( NumberBitmap[(num * 5) + i][j] )
				draw_one_pixel(x + i, y + j, c);
		}
	}
}
