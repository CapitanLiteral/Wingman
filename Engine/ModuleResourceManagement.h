#pragma once
#include "Application.h"
#include "Globals.h"
#include "Module.h"

class GameObject;

class ModuleResourceManagement : public Module
{
public:
	ModuleResourceManagement(Application* app, bool start_enabled = true);
	virtual ~ModuleResourceManagement();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void importFBX(std::string fileName);
	GameObject* loadFBX(const char* path, GameObject* root);
};

