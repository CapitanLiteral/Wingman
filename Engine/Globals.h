#pragma once
#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

#define EDITOR_FRAME_SAMPLES 50

#define RELEASE( x )\
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x )\
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }

typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

enum module
{
	BEER_TEST,
	WINDOW	
};




// Configuration -----------
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define WIN_MAXIMIZED true
#define VSYNC true

#define TITLE "Wingman game editor"
#define ORGANIZATION "UndistinguishedMan"



#define LOCAL_SAVE_DIRECTORY "data"
#define ASSETS_DIRECTORY "assets"
#define CONFIG_DIRECTORY "config"
#define MESHES_DIRECTORY "meshes"
#define MATERIAL_DIRECTORY "material"
#define ANIMATION_DIRECTORY "animation"
#define ROOT "root/"