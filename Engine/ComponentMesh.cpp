#include "Application.h"

#include "ComponentMesh.h"
#include "GameObject.h"

#include "OpenGL.h"

#include "Assimp\Assimp\include\scene.h"
#include "Assimp\Assimp\include\cfileio.h"
#include "Assimp\Assimp\include\cimport.h"
#include "Assimp\Assimp\include\postprocess.h"

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

void ComponentMesh::load(const aiMesh* mesh)
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
		UV = new float[numVertices * 3];
		memcpy(UV, mesh->mTextureCoords[0], sizeof(float) * numVertices * 3);
	}

	//Generating GL Buffers

	//vertices
	glGenBuffers(1, (GLuint*) &(idVertices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idVertices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*numVertices * 3, vertices, GL_STATIC_DRAW);

	//normals
	if (normals != nullptr)
	{
		glGenBuffers(1, (GLuint*) &(idNormals));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idNormals);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*numNormals * 3, normals, GL_STATIC_DRAW);
	}
	//indices
	glGenBuffers(1, (GLuint*) &(idIndices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t)*numIndices, indices, GL_STATIC_DRAW);

	//UV
	if (UV != nullptr)
	{
		glGenBuffers(1, (GLuint*) &(idUV));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idUV);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t)*numVertices * 3, UV, GL_STATIC_DRAW);
	}


}

void ComponentMesh::draw()
{
	glPushMatrix();
	//glMultMatrixf(parent->getLocalTransform().ptr());
	glMultMatrixf(parent->getGlobalTransform().Transposed().ptr());
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
	glPopMatrix();
}

void ComponentMesh::drawUI()
{
	//TODO
}

