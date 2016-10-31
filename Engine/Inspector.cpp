#include "Application.h"
#include "Globals.h"
#include "ModuleWindow.h"
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

	ImGui::SetNextWindowSize(ImVec2(App->window->screen_surface->w / 8 * 7, 600), ImGuiSetCond_FirstUseEver);
	//ImGui::SetNextWindowPos(ImVec2(App->window->screen_surface->w / 8 * 7, 20));
	if (!ImGui::Begin("Inspector", &active, outilnerWindowFlags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	if (App->goManager->getFocusGO() != nullptr)
	{
		ImGui::Text("Local Transform");		
		float3 position;
		float3 scale;
		Quat rot;
		App->goManager->getFocusGO()->localTransform.Decompose(position, rot, scale);
		float3 localEulerAngles = rot.ToEulerXYZ();
				
		if (ImGui::DragFloat3("Position", position.ptr(), 0.01f))
			App->goManager->getFocusGO()->setPosition(position);
		

		if (ImGui::DragFloat3("Rotation", localEulerAngles.ptr(), 0.01f))
		{			
			rot = Quat::FromEulerXYZ(localEulerAngles.x, localEulerAngles.y, localEulerAngles.z);
			App->goManager->getFocusGO()->setRotation(rot);
		}

		if (ImGui::DragFloat3("Scale", scale.ptr(), 0.01f))
			App->goManager->getFocusGO()->setScale(scale);


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ImGui::Text("Global Transform");

		float3 Gposition;
		float3 Gscale;
		Quat Grot;

		App->goManager->getFocusGO()->globalTransform.Decompose(Gposition, Grot, Gscale);

		float3 eulerAngles = Grot.ToEulerXYZ();
		ImGui::DragFloat3("GPosition", Gposition.ptr(), 0.01f);
		ImGui::DragFloat3("GRotation", eulerAngles.ptr(), 0.01f);
		ImGui::DragFloat3("GScale", Gscale.ptr(), 0.01f);

		Grot = Quat::FromEulerXYZ(eulerAngles.x, eulerAngles.y, eulerAngles.z);

		//App->goManager->getFocusGO()->setGlobalTransform(Gposition, Grot, Gscale);

	}
	
	ImGui::Spacing();



	ImGui::End();

	/////////////////////////////////////////////////////////////////////////////////////////////////


}
