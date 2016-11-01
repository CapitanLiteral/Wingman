#include "SDL\include\SDL.h"
#include "GameObject.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "MathGeoLib\include\MathGeoLib.h"


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
}

GameObject::~GameObject()
{
	for (std::vector<Component*>::iterator iterator = components.begin(); 
	iterator != components.end(); iterator++)
	{
		if ((*iterator) != nullptr)
		{
			delete (*iterator);
		}
	}

	for (std::vector<GameObject*>::iterator iterator = children.begin();
		 iterator != children.end(); iterator++)
	{
		if ((*iterator) != nullptr)
		{
			delete (*iterator);
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
	}
	for (std::vector<GameObject*>::iterator it = children.begin();
			it != children.end(); it++)
	{
		(*it)->update();
	}	
}

Component * GameObject::findComponent(ComponentType)
{
	return nullptr;
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
}
void GameObject::setRotation(Quat rotation)
{
	float3 pos;
	float3 sca;
	Quat rot;
	localTransform.Decompose(pos, rot, sca);

	localTransform = float4x4::FromTRS(pos, rotation, sca);
	refreshTransform(globalTransform);
}
void GameObject::setScale(float3 scale)
{
	float3 pos;
	float3 sca;
	Quat rot;
	localTransform.Decompose(pos, rot, sca);

	localTransform = float4x4::FromTRS(pos, rot, scale);
	refreshTransform(globalTransform);
}

void GameObject::setGlobalTransform(float3 pos, Quat rot, float3 scale)
{
	globalTransform = float4x4::FromTRS(pos, rot, scale);
}






