#pragma once
#include "statemachine.h"
#include "sprite.h"
#include "text.h"

typedef std::pair<const char*, Sprite*> ListSprite;

class Object
{
public:
	StateMachine* state;
	Sprite* sprite;
	std::map<const char*, Sprite*> sprites;

	Object() {};
	Object(int _x, int _y, ListSprite _sprite, ...){
		va_list arg;
		va_start(arg, _sprite);
		for (; _sprite.second != nullptr; _sprite = va_arg(arg, ListSprite))
		{
			sprites.emplace(_sprite);
			sprite = sprites[_sprite.first];
		}
		va_end(arg);

		x = _x;
		y = _y;

		alpha = 1.0f;
	};
	~Object() {};

	virtual void Update() {};
	virtual void Draw() { if (sprite != nullptr) drawSprite(sprite, x, y, offsetx, offsety, alpha); };
	virtual int getW() { return width; }; //width
	virtual int getH() { return height; }; //height

	virtual void AddActions(std::initializer_list<void(*)()> functions)
	{
		for (auto elem : functions)
			actions.push_back(elem);
	}


	int x;
	int y;
	double alpha ;
	std::vector<void(*)()>actions;

protected:
	int width;
	int height;

	int type;
	int offsetx, offsety;
};

class Player : public Object
{
	int direction;
	int lastdir = 1;

public:
	Player(int _x, int _y, int offsetx, int offsety, ListSprite _sprite, ...);
	~Player(){};

	void Update();
	void Draw();

	void loadSprites(ListSprite _sprite, ...);
};

class Window : public Object
{
public:
	Window(){};
	Window(int _x, int _y, int _width, int _height, ListSprite _sprite, ...);
	~Window(){};

	void Update();
	void Draw();
};

class Menu : public Object
{
	font_data font;
	Window* window;
	std::vector<const char*> lines;
	bool up, down;
public:
	unsigned int selected = 0;
	Menu(int _x, int _y, int _width, int _height, int _pages, int _h, const char* font, ListSprite _sprite, const char* _text, ...);
	~Menu(){};

	void Update();
	void Draw();	
	
	void AddActions(std::initializer_list<void(*)()> functions);
};

class Dialogue : public Object
{
	font_data font;
	Window* window;
	std::vector<const char*> pages;
	bool cont;
	unsigned int page;

public:
	Dialogue(int _x, int _y, int _width, int _height, unsigned  int _pages, int _h, const char* font, ListSprite _sprite, const char* _text, ...);
	~Dialogue(){};

	void Update();
	void Draw();
};

class Tile : public Object
{
public:
	Tile(int x, int y, Sprite* sprite);
	~Tile(){};

	void Update();
	void Draw();
};