#include <vector>

#include "Application.h"
#include "Globals.h"
#include "ModuleFBXImporter.h"
#include "GameObjectManager.h"
#include "JsonSerializer.h"

#include "Assimp\Assimp\include\scene.h"
#include "Assimp\Assimp\include\cfileio.h"
#include "Assimp\Assimp\include\cimport.h"
#include "Assimp\Assimp\include\postprocess.h"


#include "GameObject.h"

//#pragma comment (lib, "Assimp/Assimp/libx86/assimp.lib")

#include "OpenGL.h"


ModuleFBXImporter::ModuleFBXImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

bool ModuleFBXImporter::Init()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return true;
}

bool ModuleFBXImporter::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

update_status ModuleFBXImporter::Update(float dt)
{
	if (bImportMecha)
	{
		ImportMecha();
	}
	return UPDATE_CONTINUE;
}

ModuleFBXImporter::~ModuleFBXImporter()
{
}


void ModuleFBXImporter::loadFBX(const char* full_path)
{
	

}



GameObject* ModuleFBXImporter::loadNode(const aiNode* node)
{
	GameObject* retNode = NULL;

	if (node->mNumChildren == 0)
	{
		loadNode(node);
	}


	return retNode;
}

void ModuleFBXImporter::ImportMecha()
{
	App->goManager->LoadFBX("root/data/assets/meshes/MechaT.FBX");
	ImportScene();
	
	//Delete Objects imported
	App->goManager->setFocusGO(nullptr);
	GameObject* go_root = App->goManager->root;
	for (std::vector<GameObject*>::iterator iterator = go_root->children.begin(); iterator != go_root->children.end(); iterator++)
	{
		//(*iterator)->mustBeDeleted = true;
		App->goManager->toDelete.push_back((*iterator));
		//RELEASE((*iterator));
	}
	go_root->children.clear();
	//Delete Objects imported

	bImportMecha = false;
}

void ModuleFBXImporter::ImportScene()
{
	std::string output;
	JsonSerializer::Import(App->goManager->root, output, "data/library/gameObjects/gameObjects.json");
}



