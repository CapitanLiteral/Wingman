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
	void Serialize(Json::Value& root);
	void Deserialize(Json::Value& root);

	uint diffuse;
	Color color;
	uint32_t UUID;
};

