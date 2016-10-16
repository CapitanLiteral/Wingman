#include "SDL\include\SDL.h"
#include "GameObject.h"


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

void GameObject::update()
{
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