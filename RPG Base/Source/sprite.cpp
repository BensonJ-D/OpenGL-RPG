#include "sprite.h"

void drawSprite(Sprite* sprite, int x, int y, int offsetx, int offsety, double alpha)
{
	int height = sprite->height;
	int width = sprite->width;
	int top = sprite->top;
	int left = sprite->left;
	int frame = sprite->frame;
	int imgh = sprite->imgh;
	int imgw = sprite->imgw;

	glColor4ub(255, 255, 255, (int)(255 * alpha));
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sprite->textureID);

	glBegin(GL_QUADS);
		glTexCoord2d((float)(left + width * frame) / (float)imgw, (float)(top) / (float)imgh);
		glVertex2d(x + offsetx, y + offsety);
 		glTexCoord2d((float)(left + width * frame) / (float)imgw, (float)(top + height) / (float)imgh);
		glVertex2d(x + offsetx, y + offsety + height);
		glTexCoord2d((float)(left + width * (frame + 1)) / (float)imgw, (top + height) / (float)imgh);
		glVertex2d(x + offsetx + width, y + offsety + height);
		glTexCoord2d((float)(left + width * (frame + 1)) / (float)imgw, (top) / (float)imgh);
		glVertex2d(x + offsetx + width, y + offsety);
	glEnd();    
	glDisable(GL_TEXTURE_2D);
}

void playSprite(Sprite* sprite)
{
	sprite->frame = (sprite->frame + 1) % sprite->frames;
}