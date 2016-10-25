#pragma once
#include "UI_Item.h"
#include "imGUI\imgui.h"

class GameObject;
class Outliner : public UI_Item
{
public:
	Outliner();
	virtual ~Outliner();

	void draw();
	void treeNodes(GameObject* node);

	ImGuiWindowFlags outilnerWindowFlags = 0;
};

