#ifndef GRAPHICS_HEADER_INCLUDED
#define GRAPHICS_HEADER_INCLUDED

//Color constants
static const COLOR COLOR_BLACK = {0, 0, 0, 0};
static const COLOR COLOR_WHITE = {255, 255, 255, 0};

//Public functions
void render_screen();
void clear_ball( ball_t *whichBall );

#endif
