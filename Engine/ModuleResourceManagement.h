#pragma once
#include "Globals.h"
#include "Application.h"

#include "Module.h"
#include <vector>;

#include "Assimp\Assimp\include\scene.h"
#include "Assimp\Assimp\include\cfileio.h"
#include "Assimp\Assimp\include\cimport.h"
#include "Assimp\Assimp\include\postprocess.h"

class GameObject;
class ResourceMesh;

class ModuleResourceManagement : public Module
{
public:
	std::vector<ResourceMesh*> resourceMeshes;

	ModuleResourceManagement(Application* app, bool start_enabled = true);
	virtual ~ModuleResourceManagement();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void importFBX(std::string fileName);
	GameObject* loadFBX(const char* path, GameObject* root);
	void aiSceneToGameObjects(const aiScene * scene, const aiNode * node, GameObject * parent);
};

