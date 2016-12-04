#pragma once

#include <vector>

#include "Module.h"

class aiNode;
class GameObject;

class ModuleFBXImporter :
	public Module
{
public: 
	//VramVertex meshes;
public:
	ModuleFBXImporter(Application* app, bool start_enabled = true);
	bool Init();
	bool CleanUp();
	update_status Update(float dt);

	virtual ~ModuleFBXImporter();

	void loadFBX(const char* full_path);

	GameObject* loadNode(const aiNode* node);

	void ImportMecha();
	void ImportScene();

	bool bImportMecha = false;
};

