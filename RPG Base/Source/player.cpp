#pragma once
#include "objects.h"
#include "interface.h"
#include "globals.h"

class Idle : public State
{
	Player* character;
	int direction = 0;

public:
	Idle(Player* character){ this->character = character; }

	void Enter(void** args)
	{
		direction = (int)(args);
	}

	void Update(float dt) 
	{
		if (direction == 0){ character->sprite = character->sprites["face_left"]; }
		if (direction == 1){ character->sprite = character->sprites["face_right"]; }
		if (direction == 2){ character->sprite = character->sprites["face_down"]; }
		if (direction == 3){ character->sprite = character->sprites["face_up"]; }
		//sounds.at("sample")->Stop();
	}

	void Exit() {}
	void HandleInput() {
		bool moving = false;
		int goalX, goalY;

		if (Game.state.GetState("map") != Game.state.Current()) return;

		if (KeyStates.keyState[1][(int)'a']){ goalX = character->x - 32; goalY = character->y; direction = 0; moving = true; }
		if (KeyStates.keyState[1][(int)'d']){ goalX = character->x + 32; goalY = character->y; direction = 1; moving = true; }
		if (KeyStates.keyState[1][(int)'s']){ goalX = character->x; goalY = character->y + 32; direction = 2; moving = true; }
		if (KeyStates.keyState[1][(int)'w']){ goalX = character->x; goalY = character->y - 32; direction = 3; moving = true; }
		if (moving && (!level.GetCollision(goalX, goalY + 16)))
		{
			void** pointer = new void*[3]; 
			pointer[0] = (int*)goalX;
			pointer[1] = (int*)goalY;	
			pointer[2] = (int*)direction;
			character->state->Change("moving", ((void**)(pointer)));
		}
	}
};

class Move : public State
{
	Player* character;
	int direction = 0;
	int distance;
	int goalX, goalY;

public:
	Move(Player* character){ this->character = character; }

	void Enter(void** args){
		goalX = (int)(args[0]);
		goalY = (int)(args[1]);
		direction = (int)(args[2]);
		if (direction == 0){ character->sprite = character->sprites["move_left"]; }
		if (direction == 1){ character->sprite = character->sprites["move_right"]; }
		if (direction == 2){ character->sprite = character->sprites["move_down"]; }
		if (direction == 3){ character->sprite = character->sprites["move_up"]; }

		distance = 0;
		printf("Goal Coordinates: %d, %d\nCollisions at that point? %d\n", goalX, goalY, level.GetCollision(goalX, goalY));
		//sounds.at("sample")->Play();
	}

	void Update(float dt)
	{ 
		if (direction == 0) character->x -= 2;
		if (direction == 1) character->x += 2;
		if (direction == 2) character->y += 2;
		if (direction == 3) character->y -= 2;
		distance += 2;

		if (goalX == character->x && goalY == character->y) character->state->Change("idle", (void**)direction);
		if (distance % 8 == 0)	playSprite(character->sprite);
	}

	void Exit() {}
	void HandleInput() {}
};

Player::Player(int _x, int _y, int offsetx, int offsety, ListSprite _sprite, ...)
{
	va_list arg;
	va_start(arg, _sprite);
	for (; _sprite.second != nullptr; _sprite = va_arg(arg, ListSprite))
		sprites.emplace(_sprite);
	va_end(arg);

	x = _x;
	y = _y;

	this->offsetx = offsetx;
	this->offsety = offsety;

	direction = 2;
	alpha = 1.0;
	
	sprite = sprites["face_down"];	
	state = new StateMachine();
	state->Add("moving", new Move(this));
	state->Add("idle", new Idle(this));
	state->Change("idle", (void**)direction);
}

void Player::Update()
{
	state->HandleInput();
	state->Update(0);
}

void Player::Draw()
{
	Object::Draw();
}


