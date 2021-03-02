#pragma once
#include "image.h"
#include "OpenGL.h"

struct Sprite {
	int height, width;
	int top, left;
	int frame;
	int frames;
	int textureID;	
	int imgh, imgw;
	int frameTime, lastFrame;

	Sprite() {};
	Sprite(int _width, int _height, int _top, int _left, int _frames, int _frameTime, Image* image) :
		height(_height), width(_width), top(_top), left(_left), frames(_frames), frameTime(_frameTime), textureID(image->id), imgh(image->height), imgw(image->width){
		frame = 0;
		lastFrame = glutGet(GLUT_ELAPSED_TIME);
	};
};

void drawSprite(Sprite* sprite, int x, int y, int offsetx, int offsety, double alpha);
void playSprite(Sprite* sprite);
