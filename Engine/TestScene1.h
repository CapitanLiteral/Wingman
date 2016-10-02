#pragma once
#include "Module.h"
#include "Globals.h"

class TestScene1 :
	public Module
{
public:
	TestScene1(Application* app, bool start_enabled = true);
	~TestScene1();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
};

