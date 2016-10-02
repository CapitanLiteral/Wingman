#include <vector>

#include "Application.h"
#include "Globals.h"
#include "ModuleFBXImporter.h"


#include "Assimp\Assimp\include\scene.h"
#include "Assimp\Assimp\include\cfileio.h"
#include "Assimp\Assimp\include\cimport.h"
#include "Assimp\Assimp\include\postprocess.h"

#pragma comment (lib, "Assimp/Assimp/libx86/assimp.lib")

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

			if (scene->mMeshes[i]->mNumFaces * 3)
			{
				mesh->numIndices = scene->mMeshes[i]->mNumFaces * 3;
				mesh->indices = new uint[mesh->numIndices]; // Asume all are triangles
				for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
				{
					if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
					{
						SDL_Log("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&mesh->indices[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, sizeof(uint) * 3);
					}
				}
			}
		
			meshes.push_back(mesh);

			//Generating GL Buffers

			glGenBuffers(1, (GLuint*) &(meshes[i]->idVertices));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i]->idVertices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t)*meshes[i]->numVertices, meshes[i]->vertices, GL_STATIC_DRAW);

			glGenBuffers(1, (GLuint*) &(meshes[i]->idIndices));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i]->idIndices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t)*meshes[i]->numIndices, meshes[i]->indices, GL_STATIC_DRAW);

			
		}
		aiReleaseImport(scene);
	}
	else
		SDL_Log("Error loading Scene %s", full_path);

}

void ModuleFBXImporter::drawMeshes(std::vector<VramVertex*> drawMeshes)
{
	for (uint i = 0; i < drawMeshes.size(); i++)
	{
		VramVertex* m = drawMeshes.at(i);

		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, m->idVertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->idIndices);
		glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, NULL);

		glDisableClientState(GL_VERTEX_ARRAY);
	}
}