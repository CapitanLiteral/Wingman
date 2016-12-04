#pragma once
#include "Globals.h"

#include "Component.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "Color.h"

class GameObject;
class aiMesh;
class ComponentMaterial;
class ComponentMesh : public Component
{
public:
	uint idVertices = 0;
	uint numVertices = 0;
	float* vertices = nullptr;

	uint idIndices = 0;
	uint numIndices = 0;
	uint* indices = nullptr;

	uint idNormals = 0;
	uint numNormals = 0;
	float* normals = nullptr;

	uint idUV = 0;
	float* UV = nullptr;

	int textureIndex = -1; //I think this will be used on an array of textures and this will be used to bind it
	uint32_t UUID;
	uint32_t associatedUUID;
	ComponentMaterial* associatedMaterial = nullptr;
	std::string textureName;
	std::string meshName;

	#pragma region BoundingBoxes
	private:
		OBB obb;
		AABB aabb;

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
	void load(const aiMesh* mesh);
	void draw();
	
	void draw_OBB();
	void draw_AABB();

	void Serialize(Json::Value& root);
	void Import(Json::Value& root);
	void Deserialize(Json::Value& root);
	
};

