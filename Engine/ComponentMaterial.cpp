#include "Application.h"

#include "ComponentMaterial.h"

#include "ModuleTexture.h"

ComponentMaterial::ComponentMaterial(GameObject* parent) : Component(parent)
{
	
}


ComponentMaterial::~ComponentMaterial()
{
}

uint ComponentMaterial::loadTexture(const char* path)
{
	return diffuse = App->texture->loadTexture(path);
}