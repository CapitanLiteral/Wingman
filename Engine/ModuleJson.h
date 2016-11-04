#pragma once
#include "Globals.h"
#include "Application.h"
#include "Module.h"


#include "Jsoncpp\json\json.h"

class ModuleJson : public Module
{
public:
	ModuleJson(Application* app, bool start_enabled = true);
	~ModuleJson();

	
};

