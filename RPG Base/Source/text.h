#pragma once

#include "OpenGL.h"

#include <sstream>
#include <vector>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

#pragma comment(lib, "freetype.lib")
//#pragma comment(lib, "OpenAL32.lib")
//#pragma comment(lib, "alut.lib")

//This holds all of the information related to any
//freetype font that we want to create.  
struct font_data {
	float h;			///< Holds the height of the font.
	GLuint * textures;	///< Holds the texture id's 
	GLuint list_base;	///< Holds the first display list id

	//The init function will create a font of
	//of the height h from the file fname.
	void init(const char * fname, unsigned int h);

	//Free all the resources assosiated with the font.
	void clean();
};
void drawText(const font_data &ft_font, float x, float y, float scalex, float scaley, const char *string, ...);