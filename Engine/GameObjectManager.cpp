#include "Application.h"
#include "Globals.h"

#include "GameObjectManager.h"
#include "GameObject.h"
#include "ComponentMesh.h"

#include "MathGeoLib\include\MathGeoLib.h"

#include "Assimp\Assimp\include\scene.h"
#include "Assimp\Assimp\include\cfileio.h"
#include "Assimp\Assimp\include\cimport.h"
#include "Assimp\Assimp\include\postprocess.h"


GameObjectManager::GameObjectManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

GameObjectManager::~GameObjectManager()
{
}

bool GameObjectManager::Start()
{

	return true;
}

GameObject* GameObjectManager::LoadFBX(const char * path)
{
	if (path == NULL)
	{
		SDL_Log("No path");
		return NULL; //If path is NULL dont do nothing
	}

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene, scene->HasMeshes())
	{
		SDL_Log("FBX path: %s.", path);
		LoadScene(scene, scene->mRootNode, root);		
	}

	aiReleaseImport(scene);

	return root;
}

void GameObjectManager::LoadScene(const aiScene * scene, const aiNode * node, GameObject * parent)
{
	aiVector3D ai_translation;
	aiVector3D ai_scaling;
	aiQuaternion ai_rotation;
	std::string gameObjectName;

	node->mTransformation.Decompose(ai_scaling, ai_rotation, ai_translation);

	float3 position(ai_translation.x, ai_translation.y, ai_translation.z);
	float3 scale(ai_scaling.x, ai_scaling.y, ai_scaling.z);
	Quat rotation(ai_rotation.x, ai_rotation.y, ai_rotation.z, ai_rotation.w);
	
	GameObject* gameObject = new GameObject(parent, position, scale, rotation, "NoName");
	GameObject* childGameObject = NULL;

	float4x4 matrix(rotation, position);
	matrix.Scale(scale);

	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		const aiMesh* ai_mesh = scene->mMeshes[node->mMeshes[i]];
		
		if (node->mNumMeshes > 1)
		{
			gameObjectName = ai_mesh->mName.C_Str();
			if (gameObjectName.length() == 0)
			{
				gameObjectName = node->mName.C_Str();
				gameObjectName += "_submesh";
			}
			childGameObject = new GameObject(gameObject, position, scale, rotation, gameObjectName.c_str());
		}
		else
		{
			childGameObject = gameObject;
		}




	}

}
