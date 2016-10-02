#include <vector>

#include "Application.h"
#include "Globals.h"
#include "ModuleFBXImporter.h"


#include "Assimp\Assimp\include\scene.h"
#include "Assimp\Assimp\include\cfileio.h"
#include "Assimp\Assimp\include\cimport.h"
#include "Assimp\Assimp\include\postprocess.h"

#pragma comment (lib, "Assimp/Assimp/libx86/assimp.lib")


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

ModuleFBXImporter::~ModuleFBXImporter()
{
}


void ModuleFBXImporter::loadFBX(char* full_path)
{
	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		// use scene->mNumMeshes to iterate on scene->mMeshes array
		//meshes = new VramVertex[scene->mNumMeshes];

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			//Todo
			VramVertex* mesh = new VramVertex();
			aiMesh* new_mesh = scene->mMeshes[i];		

			mesh->numVertices = new_mesh->mNumVertices;
			mesh->vertices = new float[mesh->numVertices * 3];
			memcpy(mesh->vertices, new_mesh->mVertices, sizeof(float)*  mesh->numVertices * 3);
			SDL_Log("New mesh with %d vertices", mesh->numVertices);

			meshes.push_back(mesh);
			
		}
		aiReleaseImport(scene);
	}
	else
		SDL_Log("Error loading Scene %s", full_path);
}