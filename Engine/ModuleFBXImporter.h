#pragma once
#include "Module.h"
#include <vector>

class GameObject;


struct VramVertex
{
	uint idVertices = 0;
	uint numVertices = 0;
	float* vertices = nullptr;

	uint idIndices = 0;
	uint numIndices = 0;
	uint* indices = nullptr;
	
	uint idNormals = 0;
	uint numNormals = 0;
	float* normals = nullptr;
};



class ModuleFBXImporter :
	public Module
{
public: 
	//VramVertex meshes;
public:
	ModuleFBXImporter(Application* app, bool start_enabled = true);
	bool Init();
	bool CleanUp();

	virtual ~ModuleFBXImporter();

	void loadFBX(const char* full_path);
	GameObject* loadScene(const char* full_path);

	GameObject* loadNode(const aiNode* node);


	uint loadTextures();
	void drawMeshes(std::vector<VramVertex*>);


	std::vector<VramVertex*> meshes;
};

