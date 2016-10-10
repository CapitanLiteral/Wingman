#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"



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
	ImGui::ShowTestWindow();
	static bool quitSelected = false;
	static bool aboutSelected = false;
	static bool openReleaseDirectory = false;
	static bool openRepoDirectory = false;
	static bool showfpsInfo = false;


	if (aboutSelected)
	{
		ImGui::Begin("About", &aboutSelected, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("A game engine made for educational purposes by CapitanLiteral");
		ImGui::End();
	}
	if (openReleaseDirectory)
	{
		App->OpenBrowser("https://github.com/CapitanLiteral/Wingman/releases");
		openReleaseDirectory = false;
	}
	if (openRepoDirectory)
	{
		App->OpenBrowser("https://github.com/CapitanLiteral/Wingman");
		openRepoDirectory = false;
	}
	if (showfpsInfo) ShowfpsInfo(showfpsInfo);


	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Quit", NULL, &quitSelected, true);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			ImGui::MenuItem("FPS info", NULL, &showfpsInfo, true);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("About", NULL, &aboutSelected, true);
			ImGui::MenuItem("Releases", NULL, &openReleaseDirectory, true);
			ImGui::MenuItem("Repository", NULL, &openRepoDirectory, true);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	
	if (quitSelected) //Aplication closing
	{
		SDL_Log("Closing aplication from editor");
		return UPDATE_STOP;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{
	ImGui::Render();
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
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