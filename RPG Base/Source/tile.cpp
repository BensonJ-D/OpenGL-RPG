#pragma once
#include "objects.h"
#include "globals.h"

Tile::Tile(int x, int y, Sprite* sprite)
{
	this->x = x;
	this->y = y;
	this->sprite = sprite;
	alpha = 1.0f;
}

void Tile::Update()
{
}

void Tile::Draw()
{
	Object::Draw();
}


