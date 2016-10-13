#include "Application.h"
#include "Globals.h"
#include "ModuleTexture.h"

#include "Devil\include\il.h"
#include "Devil\include\ilut.h"

#pragma comment( lib, "Devil/libx86/DevIL.lib" )
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#pragma comment( lib, "Devil/libx86/ILUT.lib" )

ModuleTexture::ModuleTexture(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}


ModuleTexture::~ModuleTexture()
{
	ilShutDown();
}

uint ModuleTexture::loadTexture(char* path)
{
	ILuint ImgID = 0;
	ilGenImages(1, &ImgID);
	ilBindImage(ImgID);
	ilLoadImage(path);

	int width = ilGetInteger(IL_IMAGE_WIDTH);
	int height = ilGetInteger(IL_IMAGE_HEIGHT);
	BYTE* pixmap = new BYTE[width * height * 3];
	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGB,
				 IL_UNSIGNED_BYTE, pixmap);

	ilBindImage(0);
	ilDeleteImage(ImgID);

	return ImgID;
}
