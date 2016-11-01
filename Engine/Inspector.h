#pragma once
#include "UI_Item.h"
#include "imGUI\imgui.h"

class Inspector : public UI_Item

{
public:
	Inspector();
	~Inspector();

	void draw();
	void transform();
	void mesh();
	void material();

	ImGuiWindowFlags inspectorWindowFlags = 0;
};

