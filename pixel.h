#ifndef PIXEL_H
#define PIXEL_H

struct _pixel
{
	float r; //every "type" within/associated with a single pixel. coordinates and colors.
	float g;
	float b;
	int x;
	int y;
};

typedef struct _pixel Pixel;

#endif //incl guards
