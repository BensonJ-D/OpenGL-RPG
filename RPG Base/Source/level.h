#pragma once

#include <fstream>
#include "image.h"
#include "objects.h"
#include "sprite.h"

class Level
{
private:
	//A 2D array of Tile pointers
	std::vector<std::vector<bool> > wallMap;
	std::vector<std::vector<std::vector<Tile*> > > layerMap;

	//Width and height of level (in tiles)
	int w;
	int h;
	int tileSize;

	void SetDimensions(int w, int h);

public:
	Level(){ layerMap.resize(3); wallMap.resize(3); };
	~Level(){};

	void AddTile(int x, int y, int layer, Tile* tile);
	Tile* GetTile(unsigned int x, unsigned int y, int layer);
	bool GetCollision(unsigned int x, unsigned int y);

	void LoadLevel(std::string filename, Image* imageManager, int layer);
	void LoadCollision(std::string filename);
	void Level::Draw(unsigned int layer);

	int GetWidth();
	int GetHeight();
};