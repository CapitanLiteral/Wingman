#include "SDL\include\SDL.h"
#include "GameObject.h"



GameObject::GameObject()
{
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