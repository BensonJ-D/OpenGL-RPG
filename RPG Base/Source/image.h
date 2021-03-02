#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "OpenGL.h"

typedef long LONG; 
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned char BYTE;

class Image
{
    public:
        FILE* hfile;
		BYTE* pixelData;
        LONG height;
        LONG width;
		GLuint id;
		Image() {};
        Image(const char* Filename);
        ~Image();
};

class Bitmap: public Image
{
public:
    Bitmap(const char* Filename, bool keepData);
    ~Bitmap();
    BYTE* pixelData;
    
private:
    bool keepData;
    DWORD written;
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    BYTE* tempPixelData;
    int padding;
};

class PNG: public Image
{
public:
    PNG(const char* Filename, bool keepData);
    BYTE* pixelData;
    
private:
    bool keepData;
    DWORD written;
    BYTE* tempPixelData;
};