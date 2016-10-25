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
class Inspector;
class WindowMenus;

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
	WindowMenus* windowMenus = NULL;
	Outliner* outliner = NULL;
	Inspector* inspector = NULL;

	bool show_inspector = false;
	bool show_Window_Menus = false;
	bool show_outliner = false;

};

#endif