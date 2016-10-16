#pragma once
#include "Globals.h"

#include "Component.h"
class ComponentMesh :
	public Component
{
public:
	ComponentMesh();
	~ComponentMesh();


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


};

