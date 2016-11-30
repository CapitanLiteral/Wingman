#pragma once
#include "Component.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "IJsonSerializable.h"


class ComponentCamera :	public Component, IJsonSerializable
{

public:
	ComponentCamera(GameObject* parent);
	virtual ~ComponentCamera();
	virtual void Serialize(Json::Value& root);
	virtual void Deserialize(Json::Value& root);

	void update();

	Frustum* frustum;

	void draw();
	bool culling = true;
	bool attachCamera = false;
	uint32_t UUID;
#pragma region Debug

	bool debug = false;

#pragma endregion

};

