#pragma once
#include "Globals.h"
#include "Component.h"
#include "Color.h"

class GameObject;
class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* parent);
	virtual ~ComponentMaterial();


	uint loadTexture(const char* path);

	uint diffuse;
	Color color;
};

