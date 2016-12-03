#include "Application.h"
#include "Globals.h"

#include "GameObjectManager.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "ModuleFileSystem.h"
#include "JsonSerializer.h"

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
	if (root != nullptr) delete root;
}


bool GameObjectManager::Start()
{
	float3 position(0, 0, 0);
	float3 scale(1,1,1);
	Quat rotation(0,0,0,0);
	root = new GameObject(NULL, position, scale, rotation, "ROOT");
	GameObject* camera = new GameObject(root, position, scale, rotation, "Main Camera");
	camera->setRotation(1, 0, 0);
	//root->addChild(camera);
	camera->createComponent(CAMERA);
	currentCamera = camera;
	return true;
}

update_status GameObjectManager::Update(float dt)
{

	root->update();

	if (haveToSaveScene)
	{
		saveScene();
	}
	if (haveToLoadScene)
	{
		loadScene();
	}

	for (std::vector<GameObject*>::iterator iterator = toDelete.begin(); 
		iterator != toDelete.end(); iterator++)
	{
		RELEASE((*iterator));
	}

	return UPDATE_CONTINUE;
}

GameObject* GameObjectManager::LoadFBX(const char * path)
{
	if (path == NULL)
	{
		SDL_Log("No path");
		return NULL; //If path is NULL dont do nothing
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
		
		mesh->load(ai_mesh);
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
		

		int iii = mesh->idVertices;
	}



	for (uint i = 0; i < node->mNumChildren; ++i)
		LoadScene(scene, node->mChildren[i], gameObject);

}

GameObject * GameObjectManager::getFocusGO()
{
	return focusGO;
}

void GameObjectManager::setFocusGO(GameObject * focusGO)
{
	this->focusGO = focusGO;
}

void GameObjectManager::saveScene()
{
	std::string output;
	JsonSerializer::Serialize(root, output, "data/library/gameObjects/gameObjects.json");
	haveToSaveScene = false;
}

void GameObjectManager::loadScene()
{
	JsonSerializer::Deserialize(root, "root/data/library/gameObjects/gameObjects.json");
	haveToLoadScene = false;
}
