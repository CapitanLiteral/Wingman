#include "Application.h"
#include "Globals.h"

#include "Outliner.h"
#include "imGUI\imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
#include "OpenGL.h"

#include "GameObjectManager.h"
#include "GameObject.h"

Outliner::Outliner()
{
}


Outliner::~Outliner()
{
}

void Outliner::draw()
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

	const GameObject* root = App->goManager->root;

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick;
	flags |= ImGuiTreeNodeFlags_OpenOnArrow;

	if (ImGui::TreeNodeEx("MainNode", flags))
	{
		if (root)
		{
			std::vector<GameObject*> children = root->children;
			for (std::vector<GameObject*>::iterator it = children.begin();
				 it != children.end(); it++)
			{
				treeNodes((*it));
			}
		}

		ImGui::TreePop();
	}
	

	ImGui::End();

	/////////////////////////////////////////////////////////////////////////////////////////////////
}

void Outliner::treeNodes(GameObject* node)
{
	ImGuiTreeNodeFlags nodeFlags = 0;
	if (node->children.size() > 0)
	{
		nodeFlags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
		nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow;
	}
	else
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;

	if (ImGui::TreeNodeEx(node->name.c_str(), nodeFlags))
	{
		if (ImGui::IsItemClicked())
		{
			App->goManager->setFocusGO(node);
		}
		for (uint i = 0; i < node->children.size(); ++i)
		{
			treeNodes(node->children[i]);
		}
		ImGui::TreePop();
	}
}