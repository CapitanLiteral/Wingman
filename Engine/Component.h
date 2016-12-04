#pragma once
#include "IJsonSerializable.h"
class GameObject;


enum ComponentType
{
	MESH,
	MATERIAL,
	CAMERA
};


class Component : public IJsonSerializable
{
public:
	bool enabled = true;
	uint32_t UUID;

	Component(GameObject* parent){this->parent = parent;}
	virtual ~Component(){}

	ComponentType type;
	GameObject* parent = nullptr;

	void enable() { enabled = true;}
	void Update() { enabled = false;}
	void Serialize(Json::Value& root){};
	void Deserialize(Json::Value& root){};
	virtual void disable() {}
};

