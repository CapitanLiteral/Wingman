#pragma once
class GameObject;


enum ComponentType
{
	MESH,
	MATERIAL,
	CAMERA
};


class Component
{
public:
	bool enabled = true;

	Component(GameObject* parent){this->parent = parent;}
	virtual ~Component(){}

	ComponentType type;
	GameObject* parent = nullptr;

	void enable() { enabled = true;}
	void update() { enabled = false;}
	virtual void disable() {}
};

