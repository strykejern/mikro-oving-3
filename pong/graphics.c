#include <math.h>	//for circle drawing

#include "pong.h"
#include "graphics.h"

void circle_fill(int centerX,int centerY, int radius, COLOR c);

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
	//Clear old ball
	circle_fill(whichBall->xPos, whichBall->yPos, BALL_SIZE, COLOR_BLACK);

	//Draw new ball
	circle_fill(whichBall->xPos, whichBall->yPos, BALL_SIZE, COLOR_WHITE);

/*
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
*/

	whichBall->oldXPos = whichBall->xPos;
	whichBall->oldYPos = whichBall->yPos;
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

void circle_fill(int centerX, int centerY, int radius, COLOR c)
{
	//Bresenhams circle algrotitm
	int i;
	for( i = 0; i < radius-1; i++ )
	{
		int p = 3 - (2 * radius-i);
		int x = 0;
		int y = radius-i;
		
		for(x = 0; x <= y; x++)
		{
			if (p < 0)
			{
				p=(p+(4*x)+6);
			}
			else
			{
				y=y-1;
	
				p=p+((4*(x-y)+10));
			}
	
			draw_one_pixel(centerX + x, centerY + y, c);
			draw_one_pixel(centerX + x, centerY - y, c);
			draw_one_pixel(centerX - x, centerY + y, c);
			draw_one_pixel(centerX - x, centerY - y, c);
			draw_one_pixel(centerX + y, centerY + x, c);
			draw_one_pixel(centerX + y, centerY - x, c);
			draw_one_pixel(centerX - y, centerY + x, c);
			draw_one_pixel(centerX - y, centerY - x, c);
		}
	}
}
