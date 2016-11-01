#pragma once
#include "Globals.h"

#include "Component.h"
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
	uint numUV = 0;
	float* UV = nullptr;

	int textureIndex = -1; //I think this will be used on an array of textures and this will be used to bind it

	GameObject* parent = nullptr;
	ComponentMaterial* associatedMaterial = nullptr;


public:
	ComponentMesh(GameObject* parent);
	~ComponentMesh();

	void Update();
	void load(const aiMesh* mesh);
	void draw();
	void drawUI();



};

