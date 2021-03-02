#pragma once
#include "level.h"

void Level::SetDimensions(int w, int h)
{
	for (int layer = 0; layer < 3; layer++)
	{
		//w rows
		layerMap[layer].resize(w);

		//Each row has h columns of null Tile pointers
		for (int i = 0; i < w; i++)
		{
			layerMap[layer].at(i).resize(h, 0);
		}
	}
}

void Level::AddTile(int x, int y, int layer, Tile* tile)
{
	layerMap[layer][x][y] = tile;
}

Tile* Level::GetTile(unsigned int x, unsigned int y, int layer)
{
	if (x >= 0 && x < wallMap.size())
	{
		if (y >= 0 && y < wallMap[x].size())
		{
			return layerMap[layer][x][y];
		}
		else return nullptr;
	}
	else return nullptr;
}
bool Level::GetCollision (unsigned int x, unsigned int y)
{
	x /= tileSize;
	y /= tileSize;
	if (x >= 0 && x < wallMap.size())
	{
		if (y >= 0 && y < wallMap[x].size())
		{
			return wallMap[x][y];
		}
		else return true;
	}
	else return true;
}

void Level::LoadLevel(std::string filename, Image* image, int layer)
{
	//Loads a level from xml file
	//Load the file
	std::ifstream inFile(filename);
	tileSize = 32;	
	inFile >> w >> h;
	SetDimensions(w, h);

	for (int i = 0; i < h; i++)
	for (int ii = 0; ii < w; ii++)
	{
		int tileType;
		inFile >> tileType;
		Tile* newTile;
		if (tileType == -1)
			newTile = new Tile(ii * tileSize, i * tileSize, nullptr);
		else
			newTile = new Tile(ii * tileSize, i * tileSize, new Sprite(tileSize, tileSize, (tileType / 8) * tileSize, (tileType % 8) * tileSize, 1, 0, image));
		AddTile(ii, i, layer, newTile);
	}
}

void Level::LoadCollision(std::string filename)
{
	//Loads a level from xml file
	//Load the file
	std::ifstream inFile(filename);
	inFile >> w >> h;

	wallMap.resize(w);

	//Each row has h columns of null Tile pointers
	for (int i = 0; i < w; i++)
	{
		wallMap.at(i).resize(h, 0);
	}

	for (int i = 0; i < h; i++)
	for (int ii = 0; ii < w; ii++)
	{
		int tileType, passable;
		inFile >> tileType;
		if (tileType == 0)
			wallMap[ii][i] = true;
		else wallMap[ii][i] = false;
	}
}

void Level::Draw(unsigned int layer)
{
	if (layer < 1 || layer > layerMap.size())
		return;
	for (int i = 0; i < h; i++)
	for (int ii = 0; ii < w; ii++)
		layerMap[layer - 1][ii][i]->Draw();
}