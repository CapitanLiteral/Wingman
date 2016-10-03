#pragma once
#include "Module.h"
#include <vector>

struct VramVertex
{
	uint idVertices = 0;
	uint numVertices = 0;
	float* vertices = nullptr;

	uint idIndices = 0;
	uint numIndices = 0;
	uint* indices = nullptr;
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

	void loadFBX(char* full_path);
	uint loadTextures();
	void drawMeshes(std::vector<VramVertex*>);


	std::vector<VramVertex*> meshes;
};

