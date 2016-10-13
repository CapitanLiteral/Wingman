#pragma once
#include "Module.h"
class ModuleTexture :
	public Module
{
public:
	ModuleTexture(Application* app, bool start_enabled = true);
	virtual ~ModuleTexture();

	uint loadTexture(char* path);
};

