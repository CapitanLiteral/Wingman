#include "SDL\include\SDL.h"
#include "GameObject.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"


GameObject::GameObject(GameObject* parent, const float3 translation, const float3 scale, const Quat rotation, const char* name)
{
	this->name			= name;
	this->translation	= translation;
	this->scale			= scale;
	this->rotation		= rotation;
	this->parent		= parent;

	if (parent != NULL)
	{
		parent->addChild(this);
	}
}

GameObject::~GameObject()
{
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

float4x4 GameObject::getTransform()
{
	float4x4 ret;
	ret = float4x4::FromTRS(translation, rotation, scale);
	ret.Transpose();
	return ret;
}
