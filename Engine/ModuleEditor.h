#ifndef __MODULE_UI__
#define __MODULE_UI__

#include "Module.h"
#include "Globals.h"


union SDL_Event;

class Panel;
class PanelConsole;
class PanelGOTree;
class PanelProperties;
class PanelConfiguration;
class PanelAbout;
class PanelResources;
class PanelQuickBar;
class GameObject;
class Outliner;

class ModuleEditor : public Module
{
public:
	
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	void ShowfpsInfo(bool &showfpsInfo);

	bool CleanUp();

public:
	Outliner* outliner = NULL;

};

#endif