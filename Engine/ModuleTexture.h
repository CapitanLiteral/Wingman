#pragma once
#include <vector>

#include "Module.h"
class ModuleTexture :
	public Module
{
public:
	ModuleTexture(Application* app, bool start_enabled = true);
	virtual ~ModuleTexture();

	uint loadTexture(char* path);

	std::vector<uint> textures;

};

