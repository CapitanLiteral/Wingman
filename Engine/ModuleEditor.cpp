#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"

#include "Outliner.h"
#include "WindowMenus.h"
#include "Inspector.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
#include "OpenGL.h"



ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

// Destructor
ModuleEditor::~ModuleEditor()
{

}

// Called before render is available
bool ModuleEditor::Init()
{
	bool ret = true;

	SDL_Log("Init editor gui with imgui lib version %s", ImGui::GetVersion());

	ImGui_ImplSdlGL3_Init(App->window->GetWindow());
		
	return ret;
}

bool ModuleEditor::Start()
{
	SDL_Log("Window Menus Created");
	windowMenus = new WindowMenus();
	SDL_Log("Outliner Created");
	outliner = new Outliner();
	SDL_Log("Inspector Created");
	inspector = new Inspector();

	show_Window_Menus = true;

	return true;
}

// Called every draw update
update_status ModuleEditor::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;
	ImGui_ImplSdlGL3_NewFrame(App->window->GetWindow());
	return ret;
}

update_status ModuleEditor::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (show_Window_Menus) windowMenus->draw();
	if (show_outliner) outliner->draw();
	if (show_inspector) inspector->draw();


	if (windowMenus->QUIT)	ret = UPDATE_STOP;
	return ret;
}

update_status ModuleEditor::PostUpdate(float dt)
{
	ImGui::Render();
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
	if (outliner != NULL)
		delete outliner;
	if (inspector != NULL)
		delete inspector;
	if (windowMenus != NULL)
		delete windowMenus;

	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

void ModuleEditor::ShowfpsInfo(bool &showfpsInfo)
{
	float values[5];
	values[0] = 2;
	values[1] = 5;
	values[2] = 15;
	values[3] = 5;
	values[4] = 5;
	ImGui::Begin("Fps Window", &showfpsInfo, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS");
	ImGui::PlotHistogram("Histogram", values, 5, 0, NULL, 0.0f, 20.0f, ImVec2(0, 80));
	ImGui::End();
}