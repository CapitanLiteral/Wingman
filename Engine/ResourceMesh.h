#pragma once
#include "Application.h"
#include "Globals.h"

#include "MathGeoLib\include\MathGeoLib.h"

#include "Assimp\Assimp\include\scene.h"
#include "Assimp\Assimp\include\cfileio.h"
#include "Assimp\Assimp\include\cimport.h"
#include "Assimp\Assimp\include\postprocess.h"
#include "GameResource.h"

#pragma comment (lib, "Assimp/Assimp/libx86/assimp.lib")

class ResourceMesh : public GameResource
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

	int getRawMesh(char** buffer);
	void loadRawMesh(char* buffer);
	void loadMesh(const aiMesh* mesh);
	void loadToVram();
	int getSize() const;
};

