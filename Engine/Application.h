#pragma once

#include "Globals.h"
#include "Timer.h"
#include <list>

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleAudio;
class ModuleFileSystem;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleEditor;
class ModuleTests;
class ModuleScene;
class ModuleFBXImporter;
class ModuleTexture;
class GameObjectManager;

#include "MathGeoLib\include\MathGeoLibFwd.h"

class Application
{
public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleAudio* audio = nullptr;
	ModuleFileSystem* fs = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleCamera3D* camera = nullptr;

	ModuleEditor* editor = nullptr;
	ModuleFBXImporter* fbxImporter = nullptr;
	ModuleTexture* texture = nullptr;
	GameObjectManager* goManager = nullptr;

	float ms_frame[EDITOR_FRAME_SAMPLES];
	float framerate[EDITOR_FRAME_SAMPLES];
	int maxFPS = 0;
	bool quit;
private:

	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules;
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

	char* getOrganization();
	char* getTitle();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;