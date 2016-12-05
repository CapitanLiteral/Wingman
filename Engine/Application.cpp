#include "Application.h"


#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFileSystem.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "ModuleTexture.h"
#include "GameObjectManager.h"
#include "ModuleResourceManagement.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	fs = new ModuleFileSystem(this, true);

	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	editor = new ModuleEditor(this);

	texture = new ModuleTexture(this);
	goManager = new GameObjectManager(this);
	resourceManagement = new ModuleResourceManagement(this);
	
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(fs);
	AddModule(texture);
	AddModule(goManager);
	AddModule(audio);
	AddModule(resourceManagement);

	AddModule(editor);
	// Renderer last!
	AddModule(renderer3D);
	
	for (int n = 0; n < EDITOR_FRAME_SAMPLES; n++)
	{
		ms_frame[n] = 0;
		framerate[n] = 0;
	}
	FrameTime = -1.0f;
}
Application::~Application()
{
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend())
	{
		RELEASE (*item);
		item++;
	}
}
bool Application::Init()
{
	bool ret = true;
	totalTimer.Start();
	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		if ((*item)->IsEnabled())
			ret = (*item)->Init();
		item++;
	}

	// After all Init calls we call Start() in all modules
	SDL_Log("Application Start --------------");
	item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		if ((*item)->IsEnabled())
			ret = (*item)->Start();
		item++;
	}
	maxFPS = 0;

	ms_timer.Start();
	FPS_Timer.Start();
	return ret;
}
// ---------------------------------------------
void Application::PrepareUpdate()
{
	frameCount++;
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();

	for (int n = 0; n < EDITOR_FRAME_SAMPLES - 1; n++)
	{
		ms_frame[n] = ms_frame[n + 1];
	}
	ms_frame[EDITOR_FRAME_SAMPLES - 1] = dt;

	float tmp = FPS_Timer.Read();
	if (FPS_Timer.Read() > 1000.0f)
	{
		for (int n = 0; n < EDITOR_FRAME_SAMPLES - 1; n++)
		{
			framerate[n] = framerate[n + 1];
		}
		framerate[EDITOR_FRAME_SAMPLES-1] = frameCount;
		frameCount = 0;
		FPS_Timer.Start();
	}
	
	if (maxFPS != previous_maxFPS)
	{
		if (maxFPS < 5)
		{
			FrameTime = -1.0f;
		}
		else
		{
			FrameTime = 1000.0f / maxFPS;
		}
		previous_maxFPS = maxFPS;
	}

}
// ---------------------------------------------
void Application::FinishUpdate()
{
}
// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::iterator item = list_modules.begin();
	
	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->IsEnabled())
		{
			ret = (*item)->PreUpdate(dt);
		}
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->IsEnabled())
		{
			ret = (*item)->Update(dt);
		}
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->IsEnabled())
		{
			ret = (*item)->PostUpdate(dt);
		}
		item++;
	}

	FinishUpdate();

	if (FrameTime > 0.0001f)
	{
		while (ms_timer.Read() < FrameTime)
		{
		}
	}

	return ret;
}
bool Application::CleanUp()
{
	bool ret = true;
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		item++;
	}
	return ret;
}
bool Application::OpenBrowser(const char* link)
{
	ShellExecuteA(0, 0, "chrome.exe", link, 0, SW_SHOWMAXIMIZED);

	return true;
}
void Application::Log(char* str)
{
	
}
void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}
char * Application::getOrganization()
{
	return ORGANIZATION;
}
char * Application::getTitle()
{
	return TITLE;
}