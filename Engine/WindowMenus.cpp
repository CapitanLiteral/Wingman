#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "GameObjectManager.h"
#include "ModuleFileSystem.h"
#include "ModuleResourceManagement.h"
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
	if (creditsSelected)
	{
		showCredits();
		//App->jsonParser->print_commits_info("CapitanLiteral", "Wingman");
	}
	if (showImportFBX)
	{
		importFBX();
	}
	if (showLoadPrefab)
	{
		loadPrefab();
	}
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Save", NULL, &App->goManager->haveToSaveScene, true);
			ImGui::MenuItem("Load", NULL, &App->goManager->haveToLoadScene, true);
			ImGui::MenuItem("Quit", NULL, &quitSelected, true);
			if (ImGui::BeginMenu("Import"))
			{
				ImGui::MenuItem("FBX", NULL, &showImportFBX, true);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Prefabs"))
			{
				ImGui::MenuItem("Prefabs", NULL, &showLoadPrefab, true);
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			ImGui::MenuItem("FPS info", NULL, &showfpsInfo, true);
			ImGui::MenuItem("Outliner", NULL, &App->editor->show_outliner, true);
			ImGui::MenuItem("Inspector", NULL, &App->editor->show_inspector, true);
			ImGui::MenuItem("Settings", NULL, &App->editor->show_settings, true);
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
			ImGui::MenuItem("Credits", NULL, &creditsSelected, true);
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
void WindowMenus::showCredits()
{
	ImGuiWindowFlags outilnerWindowFlags = 0;
	outilnerWindowFlags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;

	ImGui::SetNextWindowSize(ImVec2(300, 600), ImGuiSetCond_FirstUseEver);
	if (!ImGui::Begin("Credits", &credits, outilnerWindowFlags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	ImVec4 acolor(128, 137, 237, 0);
	ImColor color(84, 172, 255);
	//SDL
	ImGui::Text("SDL");
	//Image
	//Link
	ImGui::PushStyleColor(ImGuiCol_Button, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
	if (ImGui::Button("Link"))
	{
		App->OpenBrowser("https://www.libsdl.org/");
	}
	ImGui::PopStyleColor(3);
	//Tanks!
	ImGui::Text("Thanks to SDL library to make this project possible");

	ImGui::Separator();
	//Jasoncpp
	//Image
	//Link
	//Tanks!

	ImGui::Separator();
	//Bullet
	//Image
	//Link
	//Tanks!

	ImGui::Separator();
	//MathGeoLib
	//Image
	//Link
	//Tanks!

	ImGui::Separator();
	//Assimp
	//Image
	//Link
	//Tanks!

	ImGui::Separator();
	//Devil
	//Image
	//Link
	//Tanks!

	ImGui::Separator();
	//Glew
	//Image
	//Link
	//Tanks!

	ImGui::Separator();
	//PhysFS
	//Image
	//Link
	//Tanks!



	ImGui::End();
}
void WindowMenus::importFBX()
{
	ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiSetCond_FirstUseEver);
	if (ImGui::Begin("Import FBX", NULL, ImGuiWindowFlags_MenuBar))
	{	
		// left
		std::vector<std::string> files;
		static std::string selected = "";
		ImGui::BeginChild("left pane", ImVec2(0, 200), true);		
		App->fs->getFilesOnDir("root/data/assets/fbx", files, "fbx");
		for (std::vector<std::string>::iterator iterator = files.begin();
				iterator != files.end(); iterator++)
		{
			if (ImGui::Selectable((*iterator).c_str(), selected == (*iterator)))
					selected = (*iterator);
								
		}
		ImGui::EndChild();
		if (ImGui::Button("Import")) 
		{
			App->resourceManagement->importFBX(selected);
		}
	}
	ImGui::End();
}

void WindowMenus::loadPrefab()
{
	ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiSetCond_FirstUseEver);
	if (ImGui::Begin("Prefabs", NULL, ImGuiWindowFlags_MenuBar))
	{
		// left
		std::vector<std::string> files;
		static std::string selected = "";
		ImGui::BeginChild("left pane", ImVec2(0, 200), true);
		App->fs->getFilesOnDir("root/data/library/prefabs", files, "json");
		for (std::vector<std::string>::iterator iterator = files.begin();
			 iterator != files.end(); iterator++)
		{
			if (ImGui::Selectable((*iterator).c_str(), selected == (*iterator)))
				selected = (*iterator);

		}
		ImGui::EndChild();
		if (ImGui::Button("Load"))
		{
			App->goManager->loadPrefab(selected);
		}
	}
	ImGui::End();
}
