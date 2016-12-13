#pragma once
#include "Globals.h"
#include "Application.h"

#include "Component.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "Color.h"
#include "ResourceMesh.h"

class GameObject;
class aiMesh;
class ComponentMaterial;
class ComponentMesh : public Component
{
public:
	
	ResourceMesh* mesh = nullptr;
	//uint32_t resourceMeshUUID = 0;


	int textureIndex = -1; //I think this will be used on an array of textures and this will be used to bind it
	uint32_t associatedUUID;
	ComponentMaterial* associatedMaterial = nullptr;


	#pragma region BoundingBoxes
	private:
		Color obb_color;
		Color aabb_color;

	public:
		bool drawOBB = false;
		bool drawAABB = false;

		void updateBoundingBoxes();

		OBB getOBB() const;
		void setOBB(OBB obb);

		AABB getAABB() const;
		void setAABB(AABB aabb);
	#pragma endregion

	


public:
	ComponentMesh(GameObject* parent);
	~ComponentMesh();

	void Update();
	void load();
	void draw();
	
	void draw_OBB();
	void draw_AABB();

	void Serialize(Json::Value& root);
	void Deserialize(Json::Value& root);
	
};

