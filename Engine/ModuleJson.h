#pragma once
#include "Module.h"


class ModuleJson : public Module
{
public:
	ModuleJson(Application* app, bool start_enabled = true);
	~ModuleJson();

	void read();
	bool Init();
	bool Start();
	update_status Update();

	
};

