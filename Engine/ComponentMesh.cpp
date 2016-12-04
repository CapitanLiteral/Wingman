#include "Application.h"
#include "Globals.h"
#include "ComponentMesh.h"
#include "GameObject.h"
#include "ComponentMaterial.h"
#include "OpenGL.h"
#include "GameObjectManager.h"
#include "ComponentCamera.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "ModuleFileSystem.h"

#include "Assimp\Assimp\include\scene.h"
#include "Assimp\Assimp\include\cfileio.h"
#include "Assimp\Assimp\include\cimport.h"
#include "Assimp\Assimp\include\postprocess.h"



#pragma comment (lib, "Assimp/Assimp/libx86/assimp.lib")

ComponentMesh::ComponentMesh(GameObject* parent) : Component(parent)
{
	obb.SetNegativeInfinity();
	aabb.SetNegativeInfinity();
	obb_color.Set(0.2, 1, 0.2, 1);
	aabb_color.Set(1, 0.2, 0.2, 1);
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
		ComponentCamera* camera = (ComponentCamera*)App->goManager->currentCamera->findComponent(CAMERA);
		if (camera != nullptr)
		{
			if (!camera->culling)
			{
				draw();
			}
			else
			{
				if (camera->frustum->Intersects(aabb))
				{
					draw();
				}
			}
		}
		else
		{
			draw();
		}
			
	}
}
//Load from FBX structure
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

	updateBoundingBoxes();
}
void ComponentMesh::draw()
{
	//updateBoundingBoxes();
	glPushMatrix();
	//glMultMatrixf(parent->getLocalTransform().ptr());
	glMultMatrixf(parent->getGlobalTransform().Transposed().ptr());
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
	//Texture to draw
	
	glBindTexture(GL_TEXTURE_2D, associatedMaterial->diffuse);

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

	glColor4f(associatedMaterial->color.r, associatedMaterial->color.g, associatedMaterial->color.b, associatedMaterial->color.a); //I should get fbx color TODO

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);

	//DrawWireframe for selected object
	#pragma region DrawWireframe
	if (parent->selected)
	{
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor4f(0.f, 0.8f, 0.8f, 1.f);

		if (parent->selected)
		{
			glLineWidth(5.f);
			glColor4f(0.f, 1.f, 0.f, 1.f);
		}
		else
		{
			glLineWidth(1.f);
			glDisable(GL_CULL_FACE);
		}

		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);
	}
	#pragma endregion


	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (drawAABB)
	{
		draw_AABB();
	}
	if (drawOBB)
	{	
		draw_OBB();
	}
	
}
void ComponentMesh::updateBoundingBoxes()
{
	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)vertices, numVertices);

	obb = aabb;
	obb.Transform(parent->globalTransform);
	aabb.SetFrom(obb);
}
OBB ComponentMesh::getOBB() const
{
	return obb;
}
void ComponentMesh::setOBB(OBB obb)
{
	this->obb = obb;
}
AABB ComponentMesh::getAABB() const
{
	return aabb;
}
void ComponentMesh::setAABB(AABB aabb)
{
	this->aabb = aabb;
}
void ComponentMesh::draw_OBB()
{
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glLineWidth(1.f);
	glColor4f(obb_color.r, obb_color.g, obb_color.b, obb_color.a);

	float3 vertices[8];
	obb.GetCornerPoints(vertices);

	//glColor4f(color.r, color.g, color.b, color.a);

	glBegin(GL_QUADS);

	glVertex3fv((GLfloat*)&vertices[1]);
	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[7]);
	glVertex3fv((GLfloat*)&vertices[3]);

	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[2]);
	glVertex3fv((GLfloat*)&vertices[6]);

	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[6]);
	glVertex3fv((GLfloat*)&vertices[7]);

	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[1]);
	glVertex3fv((GLfloat*)&vertices[3]);
	glVertex3fv((GLfloat*)&vertices[2]);

	glVertex3fv((GLfloat*)&vertices[3]);
	glVertex3fv((GLfloat*)&vertices[7]);
	glVertex3fv((GLfloat*)&vertices[6]);
	glVertex3fv((GLfloat*)&vertices[2]);

	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[1]);

	glEnd();

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	//glLineWidth(1.f);
}
void ComponentMesh::draw_AABB()
{
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glLineWidth(1.f);
	glColor4f(aabb_color.r, aabb_color.g, aabb_color.b, aabb_color.a);

	float3 vertices[8];
	aabb.GetCornerPoints(vertices);

	//glColor4f(color.r, color.g, color.b, color.a);

	glBegin(GL_QUADS);

	glVertex3fv((GLfloat*)&vertices[1]);
	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[7]);
	glVertex3fv((GLfloat*)&vertices[3]);

	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[2]);
	glVertex3fv((GLfloat*)&vertices[6]);

	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[6]);
	glVertex3fv((GLfloat*)&vertices[7]);

	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[1]);
	glVertex3fv((GLfloat*)&vertices[3]);
	glVertex3fv((GLfloat*)&vertices[2]);

	glVertex3fv((GLfloat*)&vertices[3]);
	glVertex3fv((GLfloat*)&vertices[7]);
	glVertex3fv((GLfloat*)&vertices[6]);
	glVertex3fv((GLfloat*)&vertices[2]);

	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[1]);

	glEnd();

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	//glLineWidth(1.f);
}
void ComponentMesh::Serialize(Json::Value & root)
{
	LCG random;
	UUID = random.Int();
	root["UUID"] = UUID;
	root["associated_material"] = associatedMaterial->UUID;
	if (textureName.at(0) != '\0')
		root["associated_material_name"] = textureName;
	else
		root["associated_material_name"] = "NoTexture";
	
	root["component_type"] = "mesh";
	root["meshName"] = meshName;
}
void ComponentMesh::Import(Json::Value & root)
{
	LCG random;
	UUID = random.Int();
	root["UUID"] = UUID;
	root["associated_material"] = associatedMaterial->UUID;
	if (textureName.at(0) != '\0')
		root["associated_material_name"] = textureName;
	else
		root["associated_material_name"] = "NoTexture";

	root["component_type"] = "mesh";

	char* buffer;
	uint size = sizeof(numVertices);
	size += sizeof(numIndices);
	size += sizeof(numNormals);
	size += sizeof(vertices) * 3 * numVertices;
	size += sizeof(indices) * 3 * numIndices;
	size += sizeof(normals) * 3 * numNormals;
	size += sizeof(UV) * 3 * numVertices;

	buffer = new char[size];
	char* cursor = buffer;

	uint bytes = sizeof(uint);

	SDL_Log("Buffer: %d", sizeof(buffer));
	SDL_Log("Size: %d", size);

	memcpy(cursor, &numVertices, bytes);
	cursor += bytes;
	memcpy(cursor, &numIndices, bytes);
	cursor += bytes;
	memcpy(cursor, &numNormals, bytes);
	cursor += bytes;

	bytes = sizeof(float) * 3 * numVertices;
	memcpy(cursor, vertices, bytes);
	cursor += sizeof(float) * 3 * numVertices;

	bytes = sizeof(uint) * 3 * numIndices;
	memcpy(cursor, indices, bytes);
	cursor += sizeof(uint) * 3 * numIndices;

	bytes = sizeof(float) * 3 * numNormals;
	memcpy(cursor, normals, bytes);
	cursor += sizeof(float) * 3 * numNormals;

	bytes = sizeof(float) * 3 * numVertices;
	memcpy(cursor, UV, bytes);

	std::string path("data/library/mesh/");
	LCG name;
	meshName.append("m_");
	meshName.append(std::to_string(name.Int()));
	meshName.append(".mesh");
	path.append(meshName);
	App->fs->save(path.c_str(), (char*)buffer, sizeof(buffer));

	root["meshName"] = meshName;

	RELEASE_ARRAY(buffer);

}
void ComponentMesh::Deserialize(Json::Value & root)
{
	UUID = root.get("UUID", -1).asInt64();
	associatedUUID = root.get("associated_material", -1).asInt64();
	//Is this really useful??? i dont think so...
	std::string jtype;
	jtype = root.get("component_type", -1).asString();

	meshName = root.get("meshName", "NoMesh").asString();

	if (jtype == "mesh")
	{
		type = MESH;
	}
}



