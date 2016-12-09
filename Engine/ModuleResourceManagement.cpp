#include "Application.h"
#include "Globals.h"
#include "ModuleResourceManagement.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ComponentMaterial.h"
#include "ResourceMesh.h"
#include "ModuleFileSystem.h"
#include "JsonSerializer.h"


ModuleResourceManagement::ModuleResourceManagement(Application* app, bool start_enabled) : Module(app, start_enabled)
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
	prefabName = App->fs->removeExtension(prefabName);
	prefabName = App->fs->addExtension(prefabName, ".json");
	std::string path("root/data/assets/fbx/");
	std::string prefabPath("data/library/prefabs/");
	std::string output;
	path.append(fileName);
	prefabPath.append(prefabName);

	GameObject* temporalRoot = new GameObject();
	temporalRoot->name = "TemporalRoot";
	temporalRoot->selializable = false;
	
	loadFBX(path.c_str(), temporalRoot);

	JsonSerializer::Serialize(temporalRoot, output, prefabPath);

}
GameObject* ModuleResourceManagement::loadFBX(const char* path, GameObject* root)
{
	GameObject* ret = root;
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
		aiSceneToGameObjects(scene, scene->mRootNode, root);
	}

	aiReleaseImport(scene);
	return ret;
}
void ModuleResourceManagement::aiSceneToGameObjects(const aiScene * scene, const aiNode * node, GameObject * parent)
{
	aiVector3D ai_translation;
	aiVector3D ai_scaling;
	aiQuaternion ai_rotation;
	std::string gameObjectName;

	node->mTransformation.Decompose(ai_scaling, ai_rotation, ai_translation);
	gameObjectName = node->mName.C_Str();
	float3 position(ai_translation.x, ai_translation.y, ai_translation.z);
	float3 scale(ai_scaling.x, ai_scaling.y, ai_scaling.z);
	Quat rotation(ai_rotation.x, ai_rotation.y, ai_rotation.z, ai_rotation.w);

	GameObject* gameObject = new GameObject(parent, position, scale, rotation, gameObjectName.c_str());

	float4x4 matrix(rotation, position);
	matrix.Scale(scale);

	//Hieracy transforms
	float3 parentPos;
	float3 parentScale;
	Quat   parentRot;

	gameObject->globalTransform = gameObject->parent->getGlobalTransform() * gameObject->getLocalTransform();
	gameObject->globalTransform = gameObject->getGlobalTransform();


	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		const aiMesh* ai_mesh = scene->mMeshes[node->mMeshes[i]];

		ComponentMesh* mesh = (ComponentMesh*)gameObject->createComponent(MESH);


		mesh->mesh->loadMesh(ai_mesh);

		char* buffer = nullptr;
		int size = 0;
		size = mesh->mesh->getRawMesh(&buffer);
		std::string path("data/library/meshes/");
		path.append(gameObject->name);
		path.append(std::to_string(i));
		path.append(".mesh");
		App->fs->save(path.c_str(), buffer, size);
		RELEASE_ARRAY(buffer);
		path.insert(0, "root/");
		App->fs->load(path.c_str(), &buffer);
		mesh->mesh->loadRawMesh(buffer);


		//PNG path && Filename
		if (scene->HasMaterials())
		{
			aiColor4D color;
			scene->mMaterials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			ComponentMaterial* material = (ComponentMaterial*)gameObject->createComponent(MATERIAL);
			material->color.Set(color.r, color.g, color.b, color.a);
			aiString ai_path;
			std::string fileName;
			scene->mMaterials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &ai_path);
			int j = 0;
			for (int i = ai_path.length - 1; i > 0; i--)
			{
				if (ai_path.data[i] != '/' && ai_path.data[i] != '\\')
				{
					j++;
				}
				else
					break;
			}
			fileName.assign(ai_path.C_Str() + ai_path.length - j,
							ai_path.C_Str() + ai_path.length);
			SDL_Log("Texture path: %s", ai_path.C_Str());
			SDL_Log("Texture name: %s", fileName.c_str());
			std::string fullPath = "../data/assets/material/";
			fullPath.append(fileName);
			material->loadTexture(fullPath.c_str());
			mesh->associatedMaterial = material;
		}
	}

	for (uint i = 0; i < node->mNumChildren; ++i)
		aiSceneToGameObjects(scene, node->mChildren[i], gameObject);

}