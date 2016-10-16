#pragma once

#include <vector>
#include <string>
#include "Component.h"
#include "MathGeoLib\include\MathGeoLib.h"

class GameObject
{
public:
	GameObject(GameObject* parent, const float3 translation, const float3 scale, const Quat rotation, const char* name);
	virtual ~GameObject();

	void update();

	Component* findComponent(ComponentType);
	Component* createComponent(ComponentType);

public:
	//Redundant information for a faster and easier way to find objects on the tree
	GameObject* parent = nullptr; // If its null this is the parent
	std::vector<GameObject*> children;

	std::vector<Component*> components;

	std::string name;

	void addChild(GameObject*);

	float3	translation = float3::zero;
	float3	scale = float3::zero;
	Quat	rotation = Quat::identity;
	
};

