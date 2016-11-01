#pragma once
#include <vector>

#include "Module.h"
class ModuleTexture :
	public Module
{
public:
	ModuleTexture(Application* app, bool start_enabled = true);
	virtual ~ModuleTexture();

	uint loadTexture(const char* path);

	//Array of loaded textures
	std::vector<uint> textures;

};

