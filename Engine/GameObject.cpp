#include "SDL\include\SDL.h"
#include "GameObject.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "OpenGL.h"

GameObject::GameObject(GameObject* parent, const float3 translation, const float3 scale, const Quat rotation, const char* name)
{
	this->name			= name;
	//this->position	= translation;
	//this->scale			= scale;
	//this->rotation		= rotation;
	this->parent		= parent;

	localTransform = float4x4::FromTRS(translation, rotation, scale);
	if (parent != NULL)
	{
		parent->addChild(this);
	}
	aabb_color.Set(0, 0, 1, 1);
}

GameObject::~GameObject()
{
	for (std::vector<Component*>::iterator iterator = components.begin(); 
	iterator != components.end(); iterator++)
	{
		if ((*iterator) != nullptr)
		{
			RELEASE(*iterator);
		}
	}

	for (std::vector<GameObject*>::iterator iterator = children.begin();
		 iterator != children.end(); iterator++)
	{
		if ((*iterator) != nullptr)
		{
			RELEASE(*iterator);
		}
	}

}

Component* GameObject::createComponent(ComponentType type)
{
	Component* ret;
	if (type == MESH)
	{
		ComponentMesh* mesh = new ComponentMesh(this);
		mesh->type = type;
		components.push_back(mesh);
		return mesh;
	}
	else if (type == MATERIAL)
	{
		ComponentMaterial* material = new ComponentMaterial(this);
		material->type = type;
		components.push_back(material);
		return material;
	}
	else if (type == CAMERA)
	{
		ComponentCamera* camera = new ComponentCamera(this);
		camera->type = type;
		components.push_back(camera);
		return camera;
	}


	return ret;
}


void GameObject::update()
{
	for (std::vector<Component*>::iterator it = components.begin();
		 it != components.end(); it++)
	{
		if ((*it)->type == MESH)
		{
			ComponentMesh* component = (ComponentMesh*)(*it);
			component->Update();
		}
		if ((*it)->type == CAMERA)
		{
			ComponentCamera* component = (ComponentCamera*)(*it);
			component->update();
		}
	}
	for (std::vector<GameObject*>::iterator it = children.begin();
			it != children.end(); it++)
	{
		(*it)->update();
	}	
	updateBoundingBoxes();
	if (drawAABB)
	{
		draw_AABB();
	}
}

Component * GameObject::findComponent(ComponentType)
{
	Component* ret = nullptr;
	for (std::vector<Component*>::iterator it = components.begin();
		 it != components.end(); it++)
	{
		if ((*it)->type == CAMERA)
		{
			ret = (*it);
		}
	}
	return ret;
}


void GameObject::addChild(GameObject* child)
{ 
	if (child != NULL)
	{
		children.push_back(child);
	}
	else
	{
		SDL_Log("Failed to add a child to %s", name);
	}
	
}

const float4x4 GameObject::getLocalTransform()
{
	//float4x4 ret;
	//ret = float4x4::FromTRS(position, rotation, scale);
	//ret.Transpose();
	return localTransform;
}

const float4x4 GameObject::getGlobalTransform()
{
	return globalTransform;
}

void GameObject::refreshTransform(float4x4 mat)
{
	if (parent != NULL)
	{
		globalTransform = parent->globalTransform * localTransform;
	}

	for (std::vector<GameObject*>::iterator iterator = children.begin(); 
	iterator != children.end(); iterator++)
	{
		(*iterator)->refreshTransform(globalTransform);
	}
	
}

void GameObject::setPosition(float3 position)
{
	float3 pos;
	float3 sca;
	Quat rot;
	localTransform.Decompose(pos, rot, sca);

	localTransform = float4x4::FromTRS(position, rot, sca);
	refreshTransform(globalTransform);

	//This need refactor TODO findComponents
	for (std::vector<Component*>::iterator iterator = components.begin(); 
	iterator != components.end(); iterator++)
	{
		if ((*iterator)->type == MESH)
		{
			ComponentMesh* mesh = (ComponentMesh*)(*iterator);
			mesh->updateBoundingBoxes();
		}
	}
}
void GameObject::setRotation(Quat rotation)
{
	float3 pos;
	float3 sca;
	Quat rot;
	localTransform.Decompose(pos, rot, sca);

	localTransform = float4x4::FromTRS(pos, rotation, sca);
	refreshTransform(globalTransform);

	//This need refactor TODO findComponents
	for (std::vector<Component*>::iterator iterator = components.begin();
		 iterator != components.end(); iterator++)
	{
		if ((*iterator)->type == MESH)
		{
			ComponentMesh* mesh = (ComponentMesh*)(*iterator);
			mesh->updateBoundingBoxes();
		}
	}
}

void GameObject::setRotation(float* rot)
{
	float3 r;
	r.Set(rot);

	while (r.x < 0)
		r.x += 360;
	while (r.y < 0)
		r.y += 360;
	while (r.z < 0)
		r.z += 360;

	while (r.x >= 360)
		r.x -= 360;
	while (r.y >= 360)
		r.y -= 360;
	while (r.z >= 360)
		r.z -= 360;

	r.x *= DEGTORAD;
	r.y *= DEGTORAD;
	r.z *= DEGTORAD;

	setRotation(Quat::FromEulerXYZ(r.x, r.y, r.z));
}

void GameObject::setRotation(float x, float y, float z)
{
	while (x < 0)
		x += 360;
	while (y < 0)
		y += 360;
	while (z < 0)
		z += 360;

	while (x >= 360)
		x -= 360;
	while (y >= 360)
		y -= 360;
	while (z >= 360)
		z -= 360;

	x *= DEGTORAD;
	y *= DEGTORAD;
	z *= DEGTORAD;

	setRotation(Quat::FromEulerXYZ(x, y, z));

}
float3 GameObject::getRotation() const
{

	float3 pos;
	float3 sca;
	Quat rot;
	localTransform.Decompose(pos, rot, sca);

	float3 ret = rot.ToEulerXYZ();

	ret.x *= RADTODEG;
	ret.y *= RADTODEG;
	ret.z *= RADTODEG;

	while (ret.x < 0)
		ret.x += 360;
	while (ret.y < 0)
		ret.y += 360;
	while (ret.z < 0)
		ret.z += 360;

	while (ret.x >= 360)
		ret.x -= 360;
	while (ret.y >= 360)
		ret.y -= 360;
	while (ret.z >= 360)
		ret.z -= 360;

	return ret;
}


float* GameObject::getEulerRot() const
{

	float3 pos;
	float3 sca;
	Quat rot;
	localTransform.Decompose(pos, rot, sca);
	
	float3 rotationEuler = rot.ToEulerXYZ();

	rotationEuler.x *= RADTODEG;
	rotationEuler.y *= RADTODEG;
	rotationEuler.z *= RADTODEG;

	while (rotationEuler.x < 0)
		rotationEuler.x += 360;
	while (rotationEuler.y < 0)
		rotationEuler.y += 360;
	while (rotationEuler.z < 0)
		rotationEuler.z += 360;

	while (rotationEuler.x >= 360)
		rotationEuler.x -= 360;
	while (rotationEuler.y >= 360)
		rotationEuler.y -= 360;
	while (rotationEuler.z >= 360)
		rotationEuler.z -= 360;

	return (float*)&rotationEuler;
}

void GameObject::setScale(float3 scale)
{
	float3 pos;
	float3 sca;
	Quat rot;
	localTransform.Decompose(pos, rot, sca);

	localTransform = float4x4::FromTRS(pos, rot, scale);
	refreshTransform(globalTransform);

	//This need refactor TODO findComponents
	for (std::vector<Component*>::iterator iterator = components.begin();
		 iterator != components.end(); iterator++)
	{
		if ((*iterator)->type == MESH)
		{
			ComponentMesh* mesh = (ComponentMesh*)(*iterator);
			mesh->updateBoundingBoxes();
		}
	}
}

void GameObject::setGlobalTransform(float3 pos, Quat rot, float3 scale)
{
	globalTransform = float4x4::FromTRS(pos, rot, scale);
}

AABB GameObject::getAABB() const
{
	return aabb;
}
void GameObject::setAABB(AABB aabb)
{
	this->aabb = aabb;
}

void GameObject::draw_AABB()
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

void GameObject::updateBoundingBoxes()
{
	aabb.SetNegativeInfinity();
	std::vector<AABB> aabbVector;

	for (std::vector<Component*>::iterator iterator = components.begin(); iterator != components.end(); iterator++)
	{
		if ((*iterator)->type == MESH)
		{
			ComponentMesh* mesh = (ComponentMesh*) (*iterator);
			aabbVector.push_back(mesh->getAABB());
		}
	}
	for (std::vector<AABB>::iterator iterator = aabbVector.begin(); iterator != aabbVector.end(); iterator++)
	{
		aabb.Enclose((*iterator));
	}

	//aabb.Transform(parent->globalTransform);
	
}