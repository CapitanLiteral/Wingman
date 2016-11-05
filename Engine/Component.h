#pragma once
class GameObject;


enum ComponentType
{
	MESH,
	MATERIAL
};


class Component
{
public:
	bool enabled = true;

	Component(GameObject* parent){this->parent = parent;}
	virtual ~Component(){}

	ComponentType type;
	GameObject* parent = nullptr;

	virtual void enable() {}
	virtual void update() {}
	virtual void disable() {}
};

