#include <stdlib.h>	//for rand()

#include "pong.h"
#include "graphics.h"

static const bool ballBitmap[10][10] = {
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

static const bool playerBitmap[5][27] = {
	{1,1,1,0, 0, 1,0,0, 0, 0,1,1,0, 0, 1,0,1, 0, 1,1,1,1, 0, 1,1,1,0,},
	{1,0,0,1, 0, 1,0,0, 0, 1,0,0,1, 0, 1,0,1, 0, 1,0,0,0, 0, 1,0,0,1,},
	{1,1,1,0, 0, 1,0,0, 0, 1,1,1,1, 0, 0,1,0, 0, 1,1,1,0, 0, 1,1,1,0,},
	{1,0,0,0, 0, 1,0,0, 0, 1,0,0,1, 0, 0,1,0, 0, 1,0,0,0, 0, 1,0,1,0,},
	{1,0,0,0, 0, 1,1,1, 0, 1,0,0,1, 0, 0,1,0, 0, 1,1,1,1, 0, 1,0,0,1,}
};

static const bool wins_bitmap[10][34] = {
	{1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,1,1,0,1,1,0,0,0,0,1,1,0,0,1,1,1,1,1,0},
	{1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,1,1,0,1,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1},
	{1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,1,1,0,1,1,1,1,0,0,1,1,0,1,1,0,0,0,1,1},
	{0,1,1,0,0,1,1,1,1,0,0,1,1,0,0,1,1,0,1,1,1,1,0,0,1,1,0,1,1,0,0,0,0,0},
	{0,1,1,0,0,1,1,1,1,0,0,1,1,0,0,1,1,0,1,1,0,1,1,0,1,1,0,0,1,1,1,0,0,0},
	{0,0,1,1,0,1,0,0,1,0,1,1,0,0,0,1,1,0,1,1,0,1,1,0,1,1,0,0,0,0,1,1,1,0},
	{0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,1,1,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,1,1},
	{0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,1,1,0,0,1,1,1,1,0,1,1,0,0,0,1,1},
	{0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,1,1,0,0,0,1,1,1,0,1,1,0,0,0,1,1},
	{0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,1,1,1,1,1,0},
};

static const bool pong_bitmap[10][34] = {
	{1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,0,0,1,1},
	{1,1,1,1,1,0,1,1,0,1,1,0,1,1,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,0,0,1,1},
	{1,1,0,1,1,0,1,1,0,1,1,0,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1},
	{1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1},
	{1,1,1,1,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,1,1,0,0,1,1,0,0,1,1,1,1,0,1,1},
	{1,1,1,0,0,0,1,1,0,1,1,0,1,1,0,0,0,1,1,1,1,0,0,1,1,0,0,1,1,1,1,0,1,1},
	{1,1,0,0,0,0,1,1,0,1,1,0,1,1,0,0,0,1,1,1,1,0,0,1,1,1,0,0,0,1,1,0,0,0},
	{1,1,0,0,0,0,1,1,0,1,1,0,1,1,0,0,0,0,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0},
	{1,1,1,0,0,0,1,1,1,1,1,0,1,1,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,1,1},
	{1,1,1,1,0,0,0,1,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,1,0,1,1},
};

static const bool numberBitmap[40][5] = {
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

//Private functions
void draw_paddle( paddle_t *whichPaddle );
void draw_ball( ball_t *whichBall );
void draw_number(const int x, const int y, int num, const COLOR c);
void draw_player_text(const int x, const int y, const COLOR c);

//Draw one paddle
void draw_paddle( paddle_t *whichPaddle )
{
	int i, j;

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

//This clears the pong ball
void clear_ball( ball_t *whichBall )
{
	int x, y;

	for( x = 0; x < BALL_SIZE; x++ )
	{
		for( y = 0; y < BALL_SIZE; y++ )
		{
			if( ballBitmap[x][y] ) 
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
			if( ballBitmap[x][y] ) 
				draw_one_pixel( whichBall->xPos+x-BALL_SIZE/2, whichBall->yPos+y-BALL_SIZE/2, COLOR_WHITE );
		}
	}

	whichBall->oldXPos = whichBall->xPos;
	whichBall->oldYPos = whichBall->yPos;
}

//Draws PLAYER on the screen
void draw_player_text(const int x, const int y, const COLOR c)
{
	int i;
	int j;
	for(j = 0; j < 5; j++)
	{
		for(i = 0; i < 27; i++)
		{
			if(playerBitmap[j][i]) draw_one_pixel(x + i, y + j, c);
		}
	}
}

//Draws WIN on the screen
void draw_wins_text(const int x, const int y, const COLOR c)
{
	//10x34
	int i, j;
	for( j = 0; j < 10; ++j )
	{
		for( i = 0; i < 34; ++i )
		{
			if ( wins_bitmap[j][i] )
				draw_one_pixel( x + i, y + j, c );
		}
	}
}


//Draws PONG on the screen
void draw_pong_text(const int x, const int y, const COLOR c)
{
	//10x34
	int i, j;
	for( j = 0; j < 10; ++j )
	{
		for( i = 0; i < 34; ++i )
		{
			if ( pong_bitmap[j][i] )
				draw_one_pixel( x + i, y + j, c );
		}
	}
}

//Draw all the game components
void render_screen()
{
	int i;

	//Draw player names
	draw_player_text( 10, 10, player1.c );
	draw_number( 40, 10, 1, player1.c );
	draw_player_text( get_screen_width()-60, 10, player2.c );
	draw_number( get_screen_width()-30, 10, 2, player2.c );
	
	//Draw both paddles
	draw_paddle( &player1 );
	draw_paddle( &player2 );

	//Draw player 1 score
	draw_number( (get_screen_width()/2)-15, 10, player1.score, player1.c );

	//Draw player 2 score
	draw_number( (get_screen_width()/2)+15, 10, player2.score, player2.c );

	//Draw all active balls
	for( i = 0; i < MAX_BALLS; i++ )
	{
		if( !ballList[i].enabled ) continue;
		draw_ball( &ballList[i] );
	}

	//Show the result on the LCD
	flip_buffers();
}

void render_intro_screen()
{
	COLOR c;
	c.r = rand() % 255;
	c.g = rand() % 255;
	c.b = rand() % 255;

	draw_pong_text(get_screen_width()/2-20, get_screen_height()/2-20, c);

	//Show the result on the LCD
	flip_buffers();
}

//Draw the victory screen
void render_winning_screen( const short winningPlayer  )
{	
	COLOR c = winningPlayer == 1 ? player1.c : player2.c;
	
	draw_player_text(get_screen_width()/2-25, get_screen_height()/2, c);
	draw_number(get_screen_width()/2-5, get_screen_height()/2, winningPlayer, c);
	draw_wins_text(get_screen_width()/2-20, get_screen_height()/2+15, c);

	//Show the result on the LCD
	flip_buffers();
}

//Draw a number between 0 and 3
void draw_number(const int x, const int y, int num, const COLOR c)
{
	int i;
	int j;

	if( num > 3 ) num = 3;
	
	for(i = 0; i < 5; i++)
	{
		for(j = 0; j < 5; j++)
		{
			if( numberBitmap[(num * 5) + i][j] )
				draw_one_pixel(x + i, y + j, c);
		}
	}
}
