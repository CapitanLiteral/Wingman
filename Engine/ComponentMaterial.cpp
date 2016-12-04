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
	LCG random;
	UUID = random.Int();
	std::string uuid_str = std::to_string(UUID);
	root["UUID"] = UUID;
	root["component_type"] = "material";
	for (int i = 0; i < 4; i++)
	{
		root["color"].append(*(&color + i));
	} 
}
void ComponentMaterial::Deserialize(Json::Value & root)
{
	UUID = root.get("UUID", -1).asInt64();

	//Is this really useful??? i dont think so...
	std::string jtype;
	jtype = root.get("component_type", -1).asString();
	if (jtype == "material")
	{
		type = MATERIAL;
	}
	//Color
	Json::Value jcolor = root.get("color", 0);
	float c[4];
	for (int i = 0; i != jcolor.size(); i++)
		*(c + i) = jcolor[i].asFloat();
	color.Set(c[0], c[2], c[2], c[3]);

}
