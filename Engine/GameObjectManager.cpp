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


GameObjectManager::GameObjectManager(Application app, bool start_enabled) : Module(app, start_enabled)
{
}

GameObjectManager::~GameObjectManager()
{
}

bool GameObjectManager::Start()
{


}

void GameObjectManager::LoadScene(const aiScene * scene, const aiNode * node, GameObject * parent)
{
	aiVector3D ai_translation;
	aiVector3D ai_scaling;
	aiQuaternion ai_rotation;

	node->mTransformation.Decompose(ai_scaling, ai_rotation, ai_translation);

	float3 position(ai_translation.x, ai_translation.y, ai_translation.z);
	float3 scale(ai_scaling.x, ai_scaling.y, ai_scaling.z);
	Quat rotation(ai_rotation.x, ai_rotation.y, ai_rotation.z, ai_rotation.w);
	
	GameObject* gameObject = new GameObject(parent, position, scale, rotation, "NoName");

	float4x4 matrix(rotation, position);
	matrix.Scale(scale);

	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		const aiMesh* ai_mesh = scene->mMeshes[node->mMeshes[i]];
	}

}
