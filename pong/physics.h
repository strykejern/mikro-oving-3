#ifndef PHYSICS_HEADER_INCLUDED
#define PHYSICS_HEADER_INCLUDED

//Public functions
bool paddle_collides( paddle_t *whichPaddle, ball_t *whichBall );
bool ball_collides( ball_t *firstBall, ball_t *secondBall );
void do_ball_collision( ball_t * whichBall );
void update_physics();
void reset_players();
void reset_ball( ball_t *whichBall );

#endif
