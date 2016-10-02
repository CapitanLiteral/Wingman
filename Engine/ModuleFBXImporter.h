#pragma once
#include "Module.h"
#include <vector>

struct VramVertex
{
	uint32_t idVertices = 0;
	uint32_t numVertices = 0;
	float* vertices = nullptr;

	uint32_t idIndices = 0;
	uint32_t numIndices = 0;
	uint32_t* indices = nullptr;
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
	std::vector<VramVertex*> meshes;
};

