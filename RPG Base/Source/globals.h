#pragma once

#define PI 3.14159265

#include "text.h"
#include "image.h"
#include "objects.h"
#include "sprite.h"
#include "statemachine.h"
#include "level.h"
#include "audio.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <map>

const int MAX_LAYERS = 5;
const int BG_LAYER = 0;
const int UI_LAYER = MAX_LAYERS - 1;

void AddObject(int Layer, const char* name, Object* Thing);

struct Input {
	bool keyState[2][246];
	int mouse_x, mouse_y, mouseState[3];
};

extern std::vector<std::map<const char*, Object*>> objects;
extern std::map<const char*, Image*> images;
extern std::map<const char*, Audio*> sounds;
extern Input KeyStates;
extern Level level;