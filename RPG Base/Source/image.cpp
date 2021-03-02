#include "image.h"
#include "png.h"

#pragma comment(lib, "zlib.lib")
#pragma comment(lib, "libpng16.lib")

int file_read = 0;

void png_read(png_structp png_ptr, png_bytep data, png_size_t length)
{
	png_size_t check;

	png_voidp file = png_get_io_ptr(png_ptr);

	if (png_ptr == NULL)
		return;

	/* fread() returns 0 on error, so it is OK to store this in a png_size_t
	* instead of an int, which is what fread() actually returns.
	*/
	check = fread(data, 1, length, ((FILE*)file));

	file_read += length;

	if (check != length)
		png_error(png_ptr, "Read Error");
}

Image::Image(const char* Filename)
{ 
    fopen_s(&hfile, (char*)Filename, "rb");
    if(hfile == NULL) 
    {
        printf("Path name: %s\n", Filename);
        printf("Could not locate referenced image. Exiting\n");
        system("pause");
		exit(1);
    }
    BYTE Signature[8];
    fread(Signature, 8, 1, hfile);
    
    if(Signature[0] == int('B') && Signature[1] == int('M'))
    {
        printf("Bitmap\n");
        fclose(hfile);
        Bitmap* img = new Bitmap(Filename, true);   
		height = img->height;
		width = img->width;
		id = img->id;
		delete img;
    }
    
    else if(Signature[0] == 137 && Signature[1] == 80 && Signature[2] == 78 && Signature[3] == 71 &&
       Signature[4] == 13 && Signature[5] == 10 && Signature[6] == 26 && Signature[7] == 10)
    {
        printf("PNG\n");
        fclose(hfile);
		PNG* img = new PNG(Filename,  true);
		height = img->height;
		width = img->width;
		id = img->id;
		delete img;
    }
    else
        printf("Unsupported format\n");
    fclose(hfile);
};

Image::~Image(){};

//Load image and allocate texture entity, assumes bitmap and 24bit data
Bitmap::Bitmap(const char* Filename, bool keepData)
{
    //char* buffer;
    glEnable(GL_TEXTURE_2D);
    
    printf("Creating temporary data...\n");
    //Open bitmap file
    fopen_s(&hfile, (char*)Filename, "rb");
    if(hfile == NULL) 
    {
        //printf("Path name: %s\n", path);
        printf("Could not locate referenced image. Exiting\n");
        exit(5);
    }
    
    //Read data to headers
    fread(&bfh, sizeof(bfh), 1, hfile);
    fread(&bih, sizeof(bih), 1, hfile);

	printf("Header: %d\nInfo: %d", sizeof(bfh), sizeof(bih));
    
    //Height and Width variables
    height = bih.biHeight;
    width = bih.biWidth;
    int size = height * width * 3;
    
    for(padding = 0; (width * 3 + padding) % 4 != 0; padding += 1);
    
    //Unconverted data
    pixelData = new BYTE[(size) + (width * height)];
    tempPixelData = new BYTE[size];
    
    printf("Organising bitmap data...\n");
    
    for(int _height = 0; _height < height; _height++)
    {
		BYTE* _tempPixelData = new BYTE[width * 3];
		BYTE* invalidData = new BYTE[size];
        
		fread(_tempPixelData, width * 3, 1, hfile);
		
		if(padding > 0)
            fread(invalidData, padding, 1, hfile);
        
		for(int _width = 0; _width < width * 3; _width++)
			tempPixelData[_height * width * 3 + _width] = _tempPixelData[_width];
        
        delete[] _tempPixelData;
        delete[] invalidData;
    }
    
    printf("Flipping image...\n");
    
    //Rearrange data from BGR to RGB and add Transparency
    for(int _height = 0; _height < (height); _height++)
        for(int _width = 0; _width < (width * 3); _width += 3)
        {
			pixelData[_height * 4 * width + (_width / 3 * 4) + 0] = tempPixelData[(height - _height - 1 ) * (width * 3) + _width + 2];
			pixelData[_height * 4 * width + (_width / 3 * 4) + 1] = tempPixelData[(height - _height - 1 ) * (width * 3) + _width + 1];
			pixelData[_height * 4 * width + (_width / 3 * 4) + 2] = tempPixelData[(height - _height - 1 ) * (width * 3) + _width + 0];
        }
    
    glLoadIdentity();
    printf("Loading image in to RAM...\n");
	glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,  GL_MODULATE);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, 
                 GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
    
    printf("Saving dimensions...\n");
    
    delete [] tempPixelData;
    
    if(keepData == false) {delete [] pixelData; printf("Removing pixel data...\n");}
    else printf("Pixel data stored...\n");
    
    glDisable(GL_TEXTURE_2D);
    printf("Deleting temporary handle...\n");
    fclose(hfile);
    printf("Operation Complete\n\n");
}

Bitmap::~Bitmap()
{
	
	printf("Deconstructing bitmap class...\n");
    
	if(keepData == true)
	{
		printf("Removing pixel data...\n");
		delete [] pixelData;
	}
    
	printf("Operation Complete\n\n");
}

PNG::PNG(const char* Filename, bool keepData)
{
    png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!pngPtr) 
    {
        printf("Couldn't initialize png read struct.\nExiting.");
        exit(7);
    }
    
    png_infop infoPtr = png_create_info_struct(pngPtr);
    if (!infoPtr) 
    {
        png_destroy_read_struct(&pngPtr, (png_infopp)0, (png_infopp)0);
        printf("Couldn't initialize png info struct.\nExiting.");
        exit(9);

    }
    fopen_s(&hfile, (char*)Filename, "rb");

	png_set_read_fn(pngPtr, (png_voidp)hfile, png_read);

	char temp[8];
	fread(temp, 1, 8, hfile);

    png_bytep* rowPtrs = NULL;
    char* data = NULL;

	png_set_sig_bytes(pngPtr, 8);
	png_read_info(pngPtr, infoPtr);

	printf("Read info struct.\n");

    png_uint_32 imgWidth =  png_get_image_width(pngPtr, infoPtr);
    png_uint_32 imgHeight = png_get_image_height(pngPtr, infoPtr);
    
    png_uint_32 bitDepth   = png_get_bit_depth(pngPtr, infoPtr);
	png_uint_32 channels = png_get_channels(pngPtr, infoPtr);
	printf("Channels: %d\n", channels);
    png_uint_32 colourType = png_get_color_type(pngPtr, infoPtr);    
    
    switch (colourType) 
    {
            
        case PNG_COLOR_TYPE_PALETTE:        
            png_set_palette_to_rgb(pngPtr);
            channels = 3;     
            break;
            
        case PNG_COLOR_TYPE_GRAY:
            if (bitDepth < 8)
                png_set_expand_gray_1_2_4_to_8(pngPtr);
        
            bitDepth = 8;
            break;
    }
    
    if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS)) 
    {        
        png_set_tRNS_to_alpha(pngPtr);      
		printf("Has Alpha\n");
        channels+=1;        
    }

    if (bitDepth == 16)
        png_set_strip_16(pngPtr);

    rowPtrs = new png_bytep[imgHeight]; 
    data = new char[imgWidth * imgHeight * bitDepth * channels / 8];
    const unsigned int stride = imgWidth * bitDepth * channels / 8;

    
    for (size_t i = 0; i < imgHeight; i++) 
    {
        png_uint_32 q = (imgHeight- i - 1) * stride;
        rowPtrs[imgHeight - 1 - i] = (png_bytep)data + q;
    }	

    png_read_image(pngPtr, rowPtrs);  
    
    glLoadIdentity();
	printf("Loading image in to RAM...\n");
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,  GL_MODULATE);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, 
                 GL_RGBA, GL_UNSIGNED_BYTE, data);
    printf("Saving dimensions...\n");
    
	height = imgHeight;
	width = imgWidth;

    delete[] (png_bytep)rowPtrs;
    png_destroy_read_struct(&pngPtr, &infoPtr,(png_infopp)0);
}