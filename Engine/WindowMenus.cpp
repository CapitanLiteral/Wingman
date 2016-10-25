#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "WindowMenus.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
#include "OpenGL.h"


WindowMenus::WindowMenus()
{
}


WindowMenus::~WindowMenus()
{
}

void WindowMenus::draw()
{
	ImGui::ShowTestWindow();


	if (aboutSelected)
	{
		ImGui::Begin("About", &aboutSelected);
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
			ImGui::MenuItem("Outliner", NULL, &App->editor->show_outliner, true);
			ImGui::MenuItem("Inspector", NULL, &App->editor->show_inspector, true);
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
		QUIT = true;
	}
}
