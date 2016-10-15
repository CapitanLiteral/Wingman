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
	uint ImgID = ilutGLLoadImage("../../Lenna.png");
	uint ret;
	if (ImgID != 0)
	{
		textures.push_back(ImgID);
		ret = ImgID;
	}
	else
	{
		SDL_Log("Error loading texture %s", path);
		ret = 0;
	}

	return ret;
}
