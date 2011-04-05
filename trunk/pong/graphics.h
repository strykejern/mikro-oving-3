#ifndef GRAPHICS_HEADER_INCLUDED
#define GRAPHICS_HEADER_INCLUDED

//Color constants
static const COLOR COLOR_BLACK = {0, 0, 0, 0};
static const COLOR COLOR_WHITE = {255, 255, 255, 0};

//Public functions
void render_screen();
void draw_paddle( struct paddle_s *whichPaddle );
void draw_ball( struct ball_s *whichBall );
void clear_ball( struct ball_s *whichBall );

#endif
