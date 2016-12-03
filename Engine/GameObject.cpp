#include "Application.h"
#include "Globals.h"
#include "GameObjectManager.h"
#include "SDL\include\SDL.h"
#include "GameObject.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "Component.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "OpenGL.h"

#include <string>

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

	for (std::vector<GameObject*>::iterator it = children.begin();
			it != children.end(); it++)
	{
		(*it)->update();
	}	
	for (std::vector<Component*>::iterator it = components.begin();
		 it != components.end(); it++)
	{
		//(*it)->Update();
		if ((*it)->type == MESH)
		{
			ComponentMesh* component = (ComponentMesh*)(*it);
			component->Update();
		}
		if ((*it)->type == CAMERA)
		{
			ComponentCamera* component = (ComponentCamera*)(*it);
			component->Update();
		}
	}
	updateBoundingBoxes();
	if (drawAABB)
	{
		draw_AABB();
	}

	//for (std::vector<GameObject*>::iterator iterator = children.begin(); iterator != children.end(); iterator++)
	//{
	//	if ((*iterator)->mustBeDeleted)
	//	{
	//		
	//		//RELEASE((*iterator));
	//	}
	//}
	

}

Component * GameObject::findComponent(ComponentType)
{
	Component* ret = nullptr;
	if (components.size() > 0)
	{
		for (std::vector<Component*>::iterator it = components.begin();
			 it != components.end(); it++)
		{
			//switch...
			if ((*it)->type == CAMERA)
			{
				ret = (*it);
			}
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

void GameObject::Serialize(Json::Value & root)
{
	
	Json::Value go;
	float3 pos;
	float3 scale;
	Quat rot;
	localTransform.Decompose(pos, rot, scale);
	//Position
	for (int i = 0; i < 3; i++)
	{
		go["pos"].append(*(pos.ptr()+i));
	}
	//Rotation
	float3 euler_rot = rot.ToEulerXYZ();
	euler_rot *= RADTODEG;	

	for (int i = 0; i < 3; i++)
	{
		go["rot"].append(*(euler_rot.ptr() + i));
	}
	//Scale
	for (int i = 0; i < 3; i++)
	{
		go["scale"].append(*(scale.ptr() + i));
	}
	//name
	go["name"] = name;
	//UUID
	if (name == "ROOT")
	{
		UUID = 0;
	}
	else
	{
		LCG random;
		UUID = random.Int();
		
	}
	go["UUID"] = UUID;
	//Parent UUID
	if (parent != nullptr)
		go["parent_UUID"] = parent->UUID;
	else
		go["parent_UUID"] = 0;
	
	std::string uuid_str = std::to_string(UUID);
	//Json::Value jcomponents;

	for (std::vector<Component*>::iterator iterator = components.begin();
		iterator != components.end(); iterator++)
	{
		if ((*iterator) != nullptr && (*iterator)->type == CAMERA)
		{	
			Json::Value jcomponents;
			(*iterator)->Serialize(jcomponents);
			go["components"].append(jcomponents);
		}
	}
	for (std::vector<Component*>::iterator iterator = components.begin();
		 iterator != components.end(); iterator++)
	{
		if ((*iterator) != nullptr && (*iterator)->type == MATERIAL)
		{
			Json::Value jcomponents;
			(*iterator)->Serialize(jcomponents);
			go["components"].append(jcomponents);
		}
	}
	for (std::vector<Component*>::iterator iterator = components.begin();
		 iterator != components.end(); iterator++)
	{
		if ((*iterator) != nullptr && (*iterator)->type == MESH)
		{
			Json::Value jcomponents;
			(*iterator)->Serialize(jcomponents);
			go["components"].append(jcomponents);
		}
	}
	//go["components"] = jcomponents;
	//root[uuid_str] = go;
	if (name != "ROOT")
		root["gameObjects"].append(go);

	//Recursive
	for (std::vector<GameObject*>::iterator iterator = children.begin();
		 iterator != children.end(); iterator++)
	{
		(*iterator)->Serialize(root);
	}
}

void GameObject::Deserialize(Json::Value & root)
{
	App->goManager->setFocusGO(nullptr);
	GameObject* go_root = App->goManager->root;
	for (std::vector<GameObject*>::iterator iterator = go_root->children.begin(); iterator != go_root->children.end(); iterator++)
	{
		//(*iterator)->mustBeDeleted = true;
		App->goManager->toDelete.push_back((*iterator));
		//RELEASE((*iterator));
	}

	go_root->children.clear();

	Json::Value gos = root.get("gameObjects", 0);
	for (int i = 0; i != gos.size(); i++)
	{
		float3 pos;
		float3 scale;
		float3 eulerRot;
		Quat rot;
		Json::Value jgo = gos[i];

		uint32_t jUUID = jgo.get("UUID", -1).asInt64();
		uint32_t jparent_UUID = jgo.get("parent_UUID", -1).asInt64();

		//Position
		Json::Value jpos = jgo.get("pos", 0);
		for (int i = 0; i != jpos.size(); i++)
			*(pos.ptr() + i) = jpos[i].asFloat();

		//Scale
		Json::Value jscale = jgo.get("scale", 0);
		for (int i = 0; i != jscale.size(); i++)
			*(scale.ptr() + i) = jscale[i].asFloat();

		//Rotation
		Json::Value jeulerRot = jgo.get("rot", 0);
		for (int i = 0; i != jeulerRot.size(); i++)
			*(eulerRot.ptr() + i) = jeulerRot[i].asFloat();

		rot = rot.FromEulerXYZ(eulerRot.x, eulerRot.y, eulerRot.z);

		std::string name = jgo["name"].asString();

		GameObject* newGo = new GameObject(go_root, pos, scale, rot, name.c_str());
		newGo->UUID = jUUID;
		newGo->parent_UUID = jparent_UUID;

		Json::Value jcomponents = jgo.get("components", 0);
		for (int i = 0; i != jcomponents.size(); i++)
		{
			Json::Value jcomponent = jcomponents[i];
			Component* component;// = new Component(newGo);
			std::string jtype = jcomponent.get("component_type", -1).asString();
			if (jtype == "camera")
			{
				component = newGo->createComponent(CAMERA);
				component->Deserialize(jcomponent);
			}
			else if (jtype == "mesh")
			{
				component = newGo->createComponent(MESH);
				component->Deserialize(jcomponent);

				for (std::vector<Component*>::iterator iterator = newGo->components.begin();
					 iterator != newGo->components.end(); iterator++)
				{
					if ((*iterator)->type == MATERIAL)
					{
						ComponentMaterial* material = (ComponentMaterial*)(*iterator);
						ComponentMesh* mesh = (ComponentMesh*)component;
						if (material->UUID == mesh->associatedUUID)
						{
							mesh->associatedMaterial = material;
						}
					}
				}
			}
			else if (jtype == "material")
			{
				component = newGo->createComponent(MATERIAL);
				component->Deserialize(jcomponent);				
			}		
		}
	}
}

GameObject * GameObject::findByUUID(uint32_t UUID)
{
	GameObject* ret = nullptr;

	if (this->UUID == UUID)
		ret = this;
	else
		for  (std::vector<GameObject*>::iterator iterator = children.begin(); 
				iterator != children.end(); iterator++)
		{	
			if (ret == nullptr)
			ret = (*iterator)->findByUUID(UUID);
		}

	return ret;
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