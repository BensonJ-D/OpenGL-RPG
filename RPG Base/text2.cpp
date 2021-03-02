/**
* From the OpenGL Programming wikibook: http://en.wikibooks.org/wiki/OpenGL_Programming
* This file is in the public domain.
* Contributors: Guus Sliepen, Sylvain Beucler
*/
#include <cstdlib>
#include <iostream>
using namespace std;

/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include "Source\text.h"

#include <ft2build.h>
#include FT_FREETYPE_H

int screen_width = 800, screen_height = 600;
GLuint program;
GLint attribute_coord;
GLint uniform_tex;
GLint uniform_color;

struct point {
	GLfloat x;
	GLfloat y;
	GLfloat s;
	GLfloat t;
};

GLuint vbo;

FT_Library ft;
FT_Face face;

const char *fontfilename = "Resources\\Fonts\\Vecna.otf";

bool init_resources() {
	/* Initialize the FreeType2 library */
	if (FT_Init_FreeType(&ft)) {
		cerr << "Could not init freetype library" << endl;
		return false;
	}

	/* Load a font */
	int fontsize = 8;

	FILE* in;
	fopen_s(&in, fontfilename, "rb");
	if (in == NULL) return NULL;

	int res_size = BUFSIZ;
	char* res = (char*)malloc(res_size);
	int nb_read_total = 0;

	while (!feof(in) && !ferror(in)) {
		if (nb_read_total + BUFSIZ > res_size) {
			if (res_size > 10 * 1024 * 1024) break;
			res_size = res_size * 2;
			res = (char*)realloc(res, res_size);
		}
		char* p_res = res + nb_read_total;
		nb_read_total += fread(p_res, 1, BUFSIZ, in);
	}

	fclose(in);
	res = (char*)realloc(res, nb_read_total + 1);
	res[nb_read_total] = '\0';
	char* font = res;

	if (font == NULL) {
		cerr << "Could not load font file " << fontfilename << endl;
		return false;
	}
	FT_Error fterr = FT_New_Memory_Face(ft, (FT_Byte*)font, fontsize, 0, &face);
	if (fterr != FT_Err_Ok) {
		cerr << "Could not init font: error 0x" << hex << fterr << endl;
		return false;
	}

	// Create the vertex buffer object
	glGenBuffers(1, &vbo);

	return true;
}

/**
* Render text using the currently loaded font and currently set font size.
* Rendering starts at coordinates (x, y), z is always 0.
* The pixel coordinates that the FreeType2 library uses are scaled by (sx, sy).
*/
void render_text(const char *text, float x, float y, float sx, float sy) {
	const char *p;
	FT_GlyphSlot g = face->glyph;

	/* Create a texture that will be used to hold one "glyph" */
	GLuint tex;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* Set up the VBO for our vertex data */
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	/* Loop through all characters */
	for (p = text; *p; p++) {
		/* Try to load and render the character */
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;

		/* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		/* Calculate the vertex and texture coordinates */
		float x2 = x + g->bitmap_left * sx;
		float y2 = -y - g->bitmap_top * sy;
		float w = g->bitmap.width * sx;
		float h = g->bitmap.rows * sy;

		point box[4] = {
			{ x2, -y2, 0, 0 },
			{ x2 + w, -y2, 1, 0 },
			{ x2, -y2 - h, 0, 1 },
			{ x2 + w, -y2 - h, 1, 1 },
		};

		/* Draw the character on the screen */
		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		/* Advance the cursor to the start of the next character */
		x += (g->advance.x >> 6) * sx;
		y += (g->advance.y >> 6) * sy;
	}

	glDeleteTextures(1, &tex);
}

