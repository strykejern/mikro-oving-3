#include <stdlib.h>	//For rand()

#include "pong.h"
#include "physics.h"

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

void do_ball_collision( ball_t * whichBall )
{
	int i = 0;

	//Move the ball
	whichBall->xPos += whichBall->xSpeed;
	whichBall->yPos += whichBall->ySpeed;

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

		//Dont collide with ourself!
		if( &ballList[i] == whichBall ) continue;

		//Dont do collisions between exact same position
		if( whichBall->xPos == ballList[i].xPos && whichBall->yPos == ballList[i].yPos ) continue;

		//Handle collisions
		if( ball_collides(whichBall, &ballList[i]) )
		{
			//move back to safe position
			short x = whichBall->xPos;
			short y = whichBall->yPos;
			whichBall->xPos += (whichBall->xPos-ballList[i].xPos)/2;
			whichBall->yPos += (whichBall->yPos-ballList[i].yPos)/2;
			ballList[i].xPos -= (x-ballList[i].xPos)/2;
			ballList[i].yPos -= (y-ballList[i].yPos)/2;

			//swap velocities!
			x = whichBall->xSpeed;
			y = whichBall->ySpeed;
			whichBall->xSpeed = ballList[i].xSpeed;
			whichBall->ySpeed = ballList[i].ySpeed;
			ballList[i].xSpeed = x;
			ballList[i].ySpeed = y;
		}
	}

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


