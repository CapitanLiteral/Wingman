#include "Application.h"

#include "ComponentMesh.h"
#include "GameObject.h"

#include "OpenGL.h"

ComponentMesh::ComponentMesh(GameObject* parent)
{
	this->parent = parent;
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

void ComponentMesh::Update()
{
	if (enabled)
	{
		draw();
	}
}

void ComponentMesh::draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//vertices
	glBindBuffer(GL_ARRAY_BUFFER, idVertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	//normals
	glBindBuffer(GL_ARRAY_BUFFER, idNormals);
	glNormalPointer(GL_FLOAT, 0, NULL);

	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndices);

	//UV
	glBindBuffer(GL_ARRAY_BUFFER, idUV);
	if (idUV != 0)
	{
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	}

	glColor4f(0.7f, 0.7f, 0.7f, 1.f); //I should get fbx color TODO

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ComponentMesh::drawUI()
{
	//TODO
}