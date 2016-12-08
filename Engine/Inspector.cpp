#include "Application.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include "Inspector.h"

#include "imGUI\imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
#include "OpenGL.h"

#include "GameObjectManager.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"

Inspector::Inspector()
{
}


Inspector::~Inspector()
{
}

void Inspector::draw()
{
	////////////////////////////////////////////////////////////////////////////////////////////////

	ImGuiIO& io = ImGui::GetIO();
	windowW = io.DisplaySize.x;
	windowH = io.DisplaySize.y;


	ImGuiWindowFlags outilnerWindowFlags = 0;
	outilnerWindowFlags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;
	//outilnerWindowFlags |= ImGuiWindowFlags_NoMove;
	//outilnerWindowFlags |= ImGuiWindowFlags_NoResize;

	ImGui::SetNextWindowSize(ImVec2(350, windowH - 20));
	ImGui::SetNextWindowPos(ImVec2(windowW - 350, 20));

	if (!ImGui::Begin("Inspector", &active, outilnerWindowFlags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	if (App->goManager->getFocusGO() != nullptr)
	{
		//ImGuiStyle style = ImGui::GetStyle();
		//ImVec4 color(0, 0, 0, 255);
		//ImGui::ColorEdit4("BGCOlor", (float*)&style.Colors[2], true);

		transform();
		mesh();
		material();
		camera();
	}
	
	ImGui::Spacing();



	ImGui::End();

	/////////////////////////////////////////////////////////////////////////////////////////////////


}

void Inspector::transform()
{
	ImGui::Text("Local Transform");
	float3 position;
	float3 scale;
	Quat rot;
	App->goManager->getFocusGO()->localTransform.Decompose(position, rot, scale);

	float3 localEulerAngles(App->goManager->getFocusGO()->getRotation());

	if (ImGui::DragFloat3("Position", position.ptr(), 0.01f))
		App->goManager->getFocusGO()->setPosition(position);


	if (ImGui::DragFloat3("Rotation", localEulerAngles.ptr(), 1.f))
	{
		//localEulerAngles *= DEGTORAD;
		//rot = Quat::FromEulerXYZ(localEulerAngles.x, localEulerAngles.y, localEulerAngles.z);
		App->goManager->getFocusGO()->setRotation(localEulerAngles.x, localEulerAngles.y, localEulerAngles.z);
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
	ImGui::Separator();
}

void Inspector::mesh()
{
	//Active
	GameObject* go = App->goManager->getFocusGO();
	int i = 0;
	if (go->aabb.IsFinite())
	{
		ImGui::Checkbox("GameObjectAABB", &go->drawAABB);
		ImGui::Separator();
	}	
	for (std::vector<Component*>::iterator iterator = go->components.begin(); 
		iterator != go->components.end(); iterator++)
	{
		if ((*iterator)->type == MESH)
		{
			ImGui::PushID(i);
			ComponentMesh* mesh = (ComponentMesh*)(*iterator);
			ImGui::Checkbox("AABB", &mesh->drawAABB); ImGui::SameLine();
			ImGui::Checkbox("OBB", &mesh->drawOBB);
			ImGui::Text("N vertex: ");
			ImGui::SameLine();
			ImGui::TextColored(ImColor(255, 80, 133), "%d", mesh->mesh->numVertices);

			ImGui::Text("N indices: ");
			ImGui::SameLine();
			ImGui::TextColored(ImColor(255, 80, 133), "%d", mesh->mesh->numIndices);

			ImGui::Text("N normals: ");
			ImGui::SameLine();
			ImGui::TextColored(ImColor(255, 80, 133), "%d", mesh->mesh->numNormals / 3);

			ImGui::Text("N UV: ");
			ImGui::SameLine();
			ImGui::TextColored(ImColor(255, 80, 133), "%d", mesh->mesh->numVertices);

			ImGui::Text("Tex ID: ");
			ImGui::SameLine();
			ImGui::TextColored(ImColor(255, 80, 133), "%d", mesh->associatedMaterial->diffuse);

			ImGui::Separator();

			ImGui::PopID();

			i++;
		}
	}

	
}

void Inspector::material()
{


	GameObject* go = App->goManager->getFocusGO();
	for (std::vector<Component*>::iterator iterator = go->components.begin();
		 iterator != go->components.end(); iterator++)
	{
		if ((*iterator)->type == MATERIAL)
		{
			ComponentMaterial* material = (ComponentMaterial*)(*iterator);
			ImGui::ColorEdit4("Color:", (float*)&material->color, false);
			glBindTexture(GL_TEXTURE_2D, material->diffuse);
			ImTextureID texture = (void*)material->diffuse;
			ImGui::Image(texture, ImVec2(150, 150));

			ImGui::Separator();
		}
	}

	
}

void Inspector::camera()
{
	GameObject* go = App->goManager->getFocusGO();
	for (std::vector<Component*>::iterator iterator = go->components.begin();
		 iterator != go->components.end(); iterator++)
	{
		if ((*iterator)->type == CAMERA)
		{
			ComponentCamera* camera = (ComponentCamera*)(*iterator);
			ImGui::Checkbox("Debug", &camera->debug);
			ImGui::Checkbox("Culling", &camera->culling);
			ImGui::Checkbox("Attach Camera", &camera->attachCamera);
			ImGui::Separator();
		}
	}
}
