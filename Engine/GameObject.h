#pragma once

#include <vector>
#include <string>
#include "Component.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "Color.h"
#include "IJsonSerializable.h"

class GameObject : public IJsonSerializable
{
public:
	GameObject(GameObject* parent, const float3 translation, const float3 scale, const Quat rotation, const char* name);
	GameObject();
	virtual ~GameObject();

	void update();

	Component* findComponent(ComponentType);
	Component* createComponent(ComponentType);

	void addChild(GameObject*);

	const float4x4 getLocalTransform();
	const float4x4 getGlobalTransform();

	void refreshTransform(float4x4 mat);

	void setPosition(float3 position);
	void setRotation(Quat rotation);
	void setRotation(float* rot);
	void setRotation(float x, float y, float z);
	void setScale(float3 scale);
	float* getEulerRot() const;
	float3 getRotation() const;

	AABB getAABB() const;
	void setAABB(AABB aabb);
	void draw_AABB();
	void updateBoundingBoxes();
	Color aabb_color;
	//Debug Things...
	void setGlobalTransform(float3 pos, Quat rot, float3 scale);

	void Serialize(Json::Value& root);
	void Deserialize(Json::Value& root);
	GameObject* findByUUID(uint32_t UUID);	
	

public:
	//Redundant information for a faster and easier way to find objects on the tree
	GameObject* parent = nullptr; // If its null this is the parent
	std::vector<GameObject*> children;

	std::vector<Component*> components;

	std::string name;

	bool selected = false;
	bool drawAABB = false;
	bool selializable = true;

	float4x4 localTransform = float4x4::identity;
	//global normal base
	float4x4 globalTransform = float4x4::identity; 	
	
	AABB aabb;

	uint32_t UUID;
	uint32_t parent_UUID;
	
};

