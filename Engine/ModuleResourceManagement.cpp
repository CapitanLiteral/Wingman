
#include "ModuleResourceManagement.h"
#include "GameObject.h"
#include "ModuleFileSystem.h"

#include "Assimp\Assimp\include\scene.h"
#include "Assimp\Assimp\include\cfileio.h"
#include "Assimp\Assimp\include\cimport.h"
#include "Assimp\Assimp\include\postprocess.h"

ModuleResourceManagement::ModuleResourceManagement(Application* app, bool start_enabled) : Module(App, start_enabled)
{
}
ModuleResourceManagement::~ModuleResourceManagement()
{
}
bool ModuleResourceManagement::Init()
{
	return true;
}
bool ModuleResourceManagement::Start()
{
	return true;
}
update_status ModuleResourceManagement::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}
update_status ModuleResourceManagement::Update(float dt)
{
	return UPDATE_CONTINUE;
}
update_status ModuleResourceManagement::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}
bool ModuleResourceManagement::CleanUp()
{	
	return true;
}
//This will generate a prefab.json, a mesh.mesh and maybe a texture.dds
void ModuleResourceManagement::importFBX(std::string fileName)
{
	std::string prefabName = fileName;
	GameObject* temporalRoot = new GameObject();
	temporalRoot->name = "TemporalRoot";



}

GameObject* ModuleResourceManagement::loadFBX(const char* path, GameObject* root)
{
	if (path == nullptr)
	{
		SDL_Log("No path");
		return nullptr; //If path is NULL dont do nothing
	}

	char* buffer;
	uint fileSize = App->fs->load(path, &buffer);
	const aiScene* scene = NULL;

	if (buffer && fileSize > 0)
	{
		scene = aiImportFileFromMemory(buffer, fileSize, aiProcessPreset_TargetRealtime_MaxQuality, "fbx");
	}
	else
	{
		SDL_Log("Error while loading fbx.");
		return NULL;
	}

	if (scene, scene->HasMeshes())
	{
		SDL_Log("FBX path: %s.", path);
		//LoadScene(scene, scene->mRootNode, root);
	}

	aiReleaseImport(scene);
}