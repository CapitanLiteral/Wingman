#include "Application.h"
#include "Globals.h"

#include "Outliner.h"
#include "imGUI\imgui.h"
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
	ImGui::SetNextWindowContentSize(ImVec2(200.f, 600.f));
	if (ImGui::Begin("Outliner", &active));
	{
		const GameObject* root = App->goManager->root;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick;

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
	}
}

void Outliner::treeNodes(GameObject* node)
{
	ImGuiTreeNodeFlags nodeFlags = 0;
	if (node->children.size() > 0)
	{
		nodeFlags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
	}
	else
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;

	if (ImGui::TreeNodeEx(node->name.c_str(), nodeFlags))
	{
		for (uint i = 0; i < node->children.size(); ++i)
		{
			treeNodes(node->children[i]);
		}
		ImGui::TreePop();
	}
}
