#include "ResourceMesh.h"



ResourceMesh::ResourceMesh()
{
}
ResourceMesh::~ResourceMesh()
{
	//Delete arrays
	if (vertices != nullptr)
	{
		delete vertices;
	}
	if (indices != nullptr)
	{
		delete indices;
	}
	if (normals != nullptr)
	{
		delete normals;
	}
	if (UV != nullptr)
	{
		delete UV;
	}
}
int ResourceMesh::getRawMesh(char ** buffer)
{
	char* pointer;
	int size = 0;
			
	size += sizeof(numIndices);
	size += sizeof(indices)  * 3 * numIndices;
			
	size += sizeof(numVertices);
	size += sizeof(vertices) * numVertices * 3;

	size += sizeof(numNormals);
	size += sizeof(normals) * numNormals * 3;
			
	size += sizeof(numUV);
	size += sizeof(UV) * numUV * 3;
						   
	//size += sizeof(obb);
	//size += sizeof(aabb);

	*buffer = new char[size];
	pointer = *buffer;
	
	memcpy(pointer, &numIndices, sizeof(uint));
	pointer += sizeof(uint);
	memcpy(pointer, &numVertices, sizeof(uint));
	pointer += sizeof(uint);
	memcpy(pointer, &numNormals, sizeof(uint));
	pointer += sizeof(uint);
	memcpy(pointer, &numUV, sizeof(uint));
	pointer += sizeof(uint);

	memcpy(pointer, indices, sizeof(uint) * numIndices);
	pointer += sizeof(uint) * numIndices;
	memcpy(pointer, vertices, sizeof(float) * 3 * numVertices);
	pointer += sizeof(float) * 3 * numIndices;
	memcpy(pointer, normals, sizeof(float) * 3 * numNormals);
	pointer += sizeof(float) * 3 * numNormals;
	memcpy(pointer, UV, sizeof(float) * 3 * numUV);
	//pointer += sizeof(UV) * 3 * numUV;

		


	return size;
}
void ResourceMesh::loadRawMesh(char * buffer)
{
	char* pointer = buffer;
	memcpy(&numIndices, pointer, sizeof(uint));
	pointer += sizeof(uint);
	memcpy(&numVertices, pointer, sizeof(uint));
	pointer += sizeof(uint);
	memcpy(&numNormals, pointer, sizeof(numNormals));
	pointer += sizeof(uint);
	memcpy(&numUV, pointer, sizeof(numUV));
	pointer += sizeof(uint);

	memcpy(indices, pointer, sizeof(uint) * numIndices);
	pointer += sizeof(uint) * numIndices;
	memcpy(vertices, pointer, sizeof(float) * 3 * numVertices);
	pointer += sizeof(float) * 3 * numVertices;
	memcpy(normals, pointer, sizeof(float) * 3 * numNormals);
	pointer += sizeof(float) * 3 * numNormals;
	memcpy(UV, pointer, sizeof(float) * 3 * numUV);
	pointer += sizeof(float) * 3 * numUV;

	SDL_Log("Num indices loaded: %d", numIndices);
	SDL_Log("Num vertices loaded: %d", numVertices);
	SDL_Log("Num Normals loaded: %d", numNormals);
	SDL_Log("Num UV loaded: %d", numUV);
	SDL_Log("First vertex: (%f, %f, %f)", *indices, *(indices + 1), *(indices + 2));
}
void ResourceMesh::loadMesh(const aiMesh * mesh)
{
	numVertices = mesh->mNumVertices;
	vertices = new float[numVertices * 3];

	memcpy(vertices, mesh->mVertices, sizeof(float)*  numVertices * 3);
	//Normals
	if (mesh->HasNormals())
	{
		numNormals = mesh->mNumVertices;
		normals = new float[numNormals * 3];

		memcpy(normals, mesh->mNormals, sizeof(float) * numNormals * 3);
	}

	if (mesh->HasFaces())
	{
		SDL_Log("New mesh with %d vertices", numVertices);

		numIndices = mesh->mNumFaces * 3;
		indices = new uint[numIndices]; // Asume all are triangles
		for (uint j = 0; j < mesh->mNumFaces; j++)
		{
			if (mesh->mFaces[j].mNumIndices != 3)
			{
				SDL_Log("WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&indices[j * 3], mesh->mFaces[j].mIndices, sizeof(uint) * 3);
			}
		}
	}
	if (mesh->HasTextureCoords(0))
	{
		numUV = numVertices;
		UV = new float[numVertices * 3];
		memcpy(UV, mesh->mTextureCoords[0], sizeof(float) * numVertices * 3);
	}
}
