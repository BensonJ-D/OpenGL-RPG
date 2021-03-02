#pragma once
#include "objects.h"
#include "interface.h"
#include "globals.h"

Menu::Menu(int _x, int _y, int _width, int _height, int _lines, int _h, const char* fontname, ListSprite _sprite, const char* _text, ...)
{
	va_list arg;
	va_start(arg, _text);
	for (int i = 0; i < _lines; _text = va_arg(arg, const char*))
	{
		lines.push_back(_text);
		i++;
	}
	va_end(arg);

	font.init(fontname, _h);
	window = new Window(_x, _y, _width, _height, _sprite, std::make_pair("", nullptr));

	alpha = 0.5;
	height = _height;
	width = _width;
	x = _x;
	y = _y;
}

void Menu::AddActions(std::initializer_list<void(*)()> functions)
{
	printf("Added action to menu\n");
	Object::AddActions(functions);
}

void Menu::Update()
{
	if (Game.state.GetState("menu") != Game.state.Current()) return;

	window->Update();
	if (!KeyStates.keyState[1][(int)'s'] && !down)
		down = true;

	if (KeyStates.keyState[1][(int)'s'] && down && selected < lines.size() - 1)
	{ 
		selected++; 
		down = false;
	}

	if (!KeyStates.keyState[1][(int)'w'] && !up)
		up = true;

	if (KeyStates.keyState[1][(int)'w'] && up && selected > 0)
	{
		selected--;
		up = false;
	}

	if (KeyStates.keyState[1][(int)' '])
	{
		if(selected < actions.size()) actions.at(selected)();
		printf("SIZE: %d\n", actions.size());
	}
}

void Menu::Draw()
{
	if (Game.state.GetState("menu") != Game.state.Current()) return;

	window->Draw();
	for (unsigned int i = 0; i < lines.size(); i++){
		if (i == selected)
		{
			glColor4ub(255, 155, 55, 155);
			drawText(font, (float)(x - 2 + 2 * font.h), (float)(y + 2 * font.h + 25 * i), 1, 1, lines[i]);
		}
		
		glColor4ub(255, 255, 255, 255);
		drawText(font, (float)(x + 2*font.h), (float)(y + 2*font.h + 25 * i), 1, 1, lines[i]);
	}
}


