#pragma once
#include "Application.h"
#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"
class ResourceMesh
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

	OBB obb;
	AABB aabb;

	ResourceMesh();
	~ResourceMesh();

	char* getRawMesh();
	void loadRawMesh(char* buffer);
};

