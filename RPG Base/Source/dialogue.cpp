#pragma once
#include "objects.h"
#include "globals.h"
//#include "OpenAL.h"


Dialogue::Dialogue(int _x, int _y, int _width, int _height, unsigned int _pages, int _h, const char* fontname, ListSprite _sprite, const char* _text, ...)
{
	va_list arg;
	va_start(arg, _text);
	for (unsigned int i = 0; i < _pages; _text = va_arg(arg, const char*))
	{
		pages.push_back(_text);
		i++;
	}
	va_end(arg);

	font.init(fontname, _h);
	window = new Window(0, 0, _width, _height, _sprite, std::make_pair("", nullptr));

	alpha = 0.5;
	height = _height;
	width = _width;
	x = _x;
	y = _y;
	page = 0;
}

void Dialogue::Update()
{
	window->Update();
	if (!KeyStates.keyState[1][(int)' '] && !cont)
		cont = true;

	if (KeyStates.keyState[1][(int)' '] && cont)
	{	
		cont = false;
		page++;

		if (page >= pages.size())
		{
			if (actions.size() > 0)
				actions[0]();
			objects[UI_LAYER]["dialogue"] = nullptr;
		}
	}

}

void Dialogue::Draw()
{
	glTranslatef((float)x, (float)y, 0);
	window->Draw();
	glLoadIdentity();
	glColor4ub(255, 255, 255, 255);
	if (page < pages.size())
		drawText(font, (float)(x + 25), (float)(y + 25), 1, 1, pages[page]);
}


