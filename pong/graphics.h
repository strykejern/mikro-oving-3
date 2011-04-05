#ifndef GRAPHICS_HEADER_INCLUDED
#define GRAPHICS_HEADER_INCLUDED

//Our color structure
typedef struct color_s
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} COLOR;

//Color constants
static const COLOR COLOR_BLACK = {0, 0, 0, 0};
static const COLOR COLOR_WHITE = {255, 255, 255, 0};

//Public functions
void draw_one_pixel(const short x, const short y, COLOR color );
void clear_screen();
void flip_buffers();
void initialize_video( const int width, const int height, const int depth );
int get_screen_width();
int get_screen_height();

#endif
