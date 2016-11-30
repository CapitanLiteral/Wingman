#include "Application.h"

#include "ComponentMaterial.h"
#include "MathGeoLib\include\MathGeoLib.h"
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

void ComponentMaterial::Serialize(Json::Value & root)
{
	Json::Value material;
	LCG random;
	UUID = random.Int();
	std::string uuid_str = std::to_string(UUID);
	material["UUID"] = UUID;
	material["component_type"] = "material";
	for (int i = 0; i < 4; i++)
	{
		material["color"].append(*(&color + i));
	}

	root[uuid_str] = material; 
}

void ComponentMaterial::Deserialize(Json::Value & root)
{
}
