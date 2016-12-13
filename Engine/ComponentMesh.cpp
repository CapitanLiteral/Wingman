#include "ComponentMesh.h"
#include "GameObject.h"
#include "ComponentMaterial.h"
#include "GameObjectManager.h"
#include "ComponentCamera.h"
#include "ModuleResourceManagement.h"

#include "OpenGL.h"

#include "Assimp\Assimp\include\scene.h"
#include "Assimp\Assimp\include\cfileio.h"
#include "Assimp\Assimp\include\cimport.h"
#include "Assimp\Assimp\include\postprocess.h"

#pragma comment (lib, "Assimp/Assimp/libx86/assimp.lib")

ComponentMesh::ComponentMesh(GameObject* parent) : Component(parent)
{
	mesh = new ResourceMesh();
	mesh->obb.SetNegativeInfinity();
	mesh->aabb.SetNegativeInfinity();
	obb_color.Set(0.2, 1, 0.2, 1);
	aabb_color.Set(1, 0.2, 0.2, 1);
}
ComponentMesh::~ComponentMesh()
{
	// Delete Buffers and restore de ID's
	if (mesh->idVertices != 0)
	{
		glDeleteBuffers(1, &mesh->idVertices);
		mesh->idVertices = 0;
	}
	if (mesh->idIndices != 0)
	{
		glDeleteBuffers(1, &mesh->idIndices);
		mesh->idIndices = 0;
	}
	if (mesh->idNormals != 0)
	{
		glDeleteBuffers(1, &mesh->idNormals);
		mesh->idNormals = 0;
	}
	if (mesh->idUV != 0)
	{
		glDeleteBuffers(1, &mesh->idUV);
		mesh->idUV = 0;
	}
	RELEASE(mesh);
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
				if (camera->frustum->Intersects(mesh->aabb))
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
	// Temporal shitty code
	load();
	draw();
}
void ComponentMesh::load()
{
	//Generating GL Buffers

	//vertices
	glGenBuffers(1, (GLuint*) &(this->mesh->idVertices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->mesh->idVertices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*this->mesh->numVertices * 3, this->mesh->vertices, GL_STATIC_DRAW);

	//normals
	if (this->mesh->normals != nullptr)
	{
		glGenBuffers(1, (GLuint*) &(this->mesh->idNormals));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->mesh->idNormals);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*this->mesh->numNormals * 3, this->mesh->normals, GL_STATIC_DRAW);
	}
	//indices
	glGenBuffers(1, (GLuint*) &(this->mesh->idIndices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->mesh->idIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t)*this->mesh->numIndices, this->mesh->indices, GL_STATIC_DRAW);

	//UV
	if (this->mesh->UV != nullptr)
	{
		glGenBuffers(1, (GLuint*) &(this->mesh->idUV));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->mesh->idUV);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t)*this->mesh->numVertices * 3, this->mesh->UV, GL_STATIC_DRAW);
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
	glBindBuffer(GL_ARRAY_BUFFER, this->mesh->idVertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	//normals
	glBindBuffer(GL_ARRAY_BUFFER, this->mesh->idNormals);
	glNormalPointer(GL_FLOAT, 0, NULL);

	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->mesh->idIndices);

	//UV
	glBindBuffer(GL_ARRAY_BUFFER, this->mesh->idUV);
	if (this->mesh->idUV != 0)
	{
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	}

	glColor4f(associatedMaterial->color.r, associatedMaterial->color.g, associatedMaterial->color.b, associatedMaterial->color.a); //I should get fbx color TODO

	glDrawElements(GL_TRIANGLES, this->mesh->numIndices, GL_UNSIGNED_INT, NULL);

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

		glDrawElements(GL_TRIANGLES, this->mesh->numIndices, GL_UNSIGNED_INT, NULL);
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
	this->mesh->aabb.SetNegativeInfinity();
	this->mesh->aabb.Enclose((float3*)this->mesh->vertices, this->mesh->numVertices);

	this->mesh->obb = this->mesh->aabb;
	this->mesh->obb.Transform(parent->globalTransform);
	this->mesh->aabb.SetFrom(this->mesh->obb);
}
OBB ComponentMesh::getOBB() const
{
	return this->mesh->obb;
}
void ComponentMesh::setOBB(OBB obb)
{
	this->mesh->obb = obb;
}
AABB ComponentMesh::getAABB() const
{
	return this->mesh->aabb;
}
void ComponentMesh::setAABB(AABB aabb)
{
	this->mesh->aabb = aabb;
}
void ComponentMesh::draw_OBB()
{
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glLineWidth(1.f);
	glColor4f(obb_color.r, obb_color.g, obb_color.b, obb_color.a);

	float3 vertices[8];
	this->mesh->obb.GetCornerPoints(vertices);

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
	this->mesh->aabb.GetCornerPoints(vertices);

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
	root["component_type"] = "mesh";
	if (mesh != nullptr)
	{
		root["resource"] = mesh->UUID;
	}
	else
	{
		root["resource"] = -1;
	}	
}
void ComponentMesh::Deserialize(Json::Value & root)
{
	UUID = root.get("UUID", -1).asInt64();
	associatedUUID = root.get("associated_material", -1).asInt64();
	//Is this really useful??? i dont think so...
	std::string jtype;
	jtype = root.get("component_type", -1).asString();
	if (jtype == "mesh")
	{
		type = MESH;
	}
	uint32_t resourceUUID;
	if (root.get("resource", -1).asInt64 != -1)
	{
		
	}
}



