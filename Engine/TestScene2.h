#pragma once
#include "Module.h"
#include "Globals.h"

class TestScene2 :
	public Module
{
public:
	TestScene2(Application* app, bool start_enabled = true);
	~TestScene2();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();


	int id = 0;
};

