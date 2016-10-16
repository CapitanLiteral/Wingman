#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Timer.h"

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleAudio;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleEditor;
class ModuleTests;
class ModulePhysics3D;
class ModuleScene;
class ModuleFBXImporter;
class ModuleTexture;
class GameObjectManager;

class TestScene1;
class TestScene2;
#include "MathGeoLib\include\MathGeoLibFwd.h"

class Application
{
public:
	ModuleWindow* window = NULL;
	ModuleInput* input = NULL;
	ModuleAudio* audio = NULL;

	ModuleRenderer3D* renderer3D = NULL;
	ModuleCamera3D* camera = NULL;

	ModuleEditor* editor = NULL;
	ModulePhysics3D* physics = NULL;
	ModuleFBXImporter* fbxImporter = NULL;
	ModuleTexture* texture = NULL;
	GameObjectManager* goManager = NULL;

	TestScene1* test1 = NULL;
	TestScene2* test2 = NULL;

	float ms_frame[EDITOR_FRAME_SAMPLES];
	float framerate[EDITOR_FRAME_SAMPLES];
	int maxFPS = 0;
	bool quit;
private:

	Timer	ms_timer;
	float	dt;
	p2List<Module*> list_modules;
	int frameCount = 0;
	Timer	FPS_Timer;
	float FrameTime = -1.0f;
	int previous_maxFPS = maxFPS;
	

public:

	Application();
	~Application();

	Timer totalTimer;
	bool Init();
	update_status Update();
	bool CleanUp();

	bool OpenBrowser(const char* link);
	void Log(char* str);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

//extern Application* App;