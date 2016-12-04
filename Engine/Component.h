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

	Component(GameObject* parent){this->parent = parent;}
	virtual ~Component(){}

	ComponentType type;
	GameObject* parent = nullptr;

	void enable() { enabled = true;}
	void Update() { enabled = false;}
	void Serialize(Json::Value& root){};
	void Import(Json::Value& root) {};
	void Deserialize(Json::Value& root){};
	virtual void disable() {}
};

