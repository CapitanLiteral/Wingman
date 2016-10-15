#pragma once

#include <vector>
#include <string>
#include "Component.h"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	void update();

	Component* findComponent(ComponentType);

public:
	//Redundant information for a faster and easier way to find objects on the tree
	GameObject* parent = nullptr; // If its null this is the parent
	std::vector<GameObject*> children;

	std::vector<Component*> components;

	std::string name;

	void addChild(GameObject*);

};

