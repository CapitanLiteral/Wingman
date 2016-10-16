#include "Application.h"

#include "ComponentMesh.h"



ComponentMesh::ComponentMesh()
{
}


ComponentMesh::~ComponentMesh()
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

	// Delete Buffers and restore de ID's
	if (idVertices != 0)
	{
		glDeleteBuffers(1, &idVertices);
		idVertices = 0;
	}
	if (idIndices != 0)
	{
		glDeleteBuffers(1, &idIndices);
		idIndices = 0;
	}
	if (idNormals != 0)
	{
		glDeleteBuffers(1, &idNormals);
		idNormals = 0;
	}
	if (idUV != 0)
	{
		glDeleteBuffers(1, &idUV);
		idUV = 0;
	}

}
}