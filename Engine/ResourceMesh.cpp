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
char * ResourceMesh::getRawMesh()
{
	return nullptr;
}
void ResourceMesh::loadRawMesh(char * buffer)
{
}
