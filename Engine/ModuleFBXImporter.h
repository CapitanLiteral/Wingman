#pragma once
#include "Module.h"
class ModuleFBXImporter :
	public Module
{
public:
	ModuleFBXImporter(Application* app, bool start_enabled = true);
	virtual ~ModuleFBXImporter();
};

