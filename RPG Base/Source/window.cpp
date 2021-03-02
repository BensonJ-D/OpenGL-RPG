#pragma once
#include "objects.h"

Window::Window(int _x, int _y, int _width, int _height, std::pair<const char*, Sprite*> _sprite, ...)
{
	va_list arg;
	va_start(arg, _sprite);
	for (; _sprite.second != nullptr; _sprite = va_arg(arg, ListSprite))
	{
		sprites.emplace(_sprite);
		sprite = sprites[_sprite.first];
	}
	va_end(arg);

	alpha = 0.4;

	height = _height;
	width = _width;
	x = _x;
	y = _y;
}

void Window::Update()
{
}

void Window::Draw()
{
	glColor4ub(255, 255, 255, (int)(255 * alpha));
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sprite->textureID);

	glBegin(GL_QUADS);
		glTexCoord2d(0, 0);
		glVertex2d(x, y);
		glTexCoord2d(0, 1);
		glVertex2d(x, y + height);
		glTexCoord2d(1, 1);
		glVertex2d(x + width, y + height);
		glTexCoord2d(1, 0);
		glVertex2d(x + width, y);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

