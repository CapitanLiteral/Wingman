

#include "Application.h"
#include "Globals.h"

#include "Inspector.h"

#include "imGUI\imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
#include "OpenGL.h"

#include "GameObjectManager.h"
#include "GameObject.h"

Inspector::Inspector()
{
}


Inspector::~Inspector()
{
}

void Inspector::draw()
{
	////////////////////////////////////////////////////////////////////////////////////////////////

	ImGuiWindowFlags outilnerWindowFlags = 0;
	outilnerWindowFlags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;

	ImGui::SetNextWindowSize(ImVec2(300, 600), ImGuiSetCond_FirstUseEver);
	if (!ImGui::Begin("New Outliner", &active, outilnerWindowFlags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	ImGui::Spacing();



	ImGui::End();

	/////////////////////////////////////////////////////////////////////////////////////////////////


}
