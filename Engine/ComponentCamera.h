#pragma once
#include "Component.h"
#include "MathGeoLib\include\MathGeoLib.h"


class ComponentCamera :	public Component
{

public:
	ComponentCamera(GameObject* parent);
	~ComponentCamera();

	void update();

	Frustum* frustum;

	void draw();

#pragma region Debug

	bool debug = false;

#pragma endregion

};
