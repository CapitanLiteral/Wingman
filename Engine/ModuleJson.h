#pragma once
#include "Module.h"
#include <json\json.h>

class ModuleJson : public Module
{
public:
	ModuleJson(Application* app, bool start_enabled = true);
	~ModuleJson();

	Json::Value read(module moduleType);
	bool Init();
	bool Start();
	update_status Update();

	
};

