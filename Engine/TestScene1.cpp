#include "Application.h"
#include "Globals.h"
#include "TestScene1.h"
#include "ModuleFBXImporter.h"
#include "ModuleTexture.h"
#include "GameObjectManager.h"
#include "GameObject.h"

#include "Primitive.h"

#include "OpenGL.h"

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "Glew/libx86/glew32.lib") /* link Microsoft OpenGL lib   */

TestScene1::TestScene1(Application* app, bool start_enabled) : Module (app, start_enabled)
{
}


TestScene1::~TestScene1()
{
}

bool TestScene1::Init()
{ 
	return true;
}
bool TestScene1::Start()
{ 
	//This is absolute route... We should change it // ERROR!!
	//App->fbxImporter->loadFBX("../DLL/MechaT.FBX");
	//App->fbxImporter->loadFBX("../DLL/Brute.fbx");
	//App->fbxImporter->loadFBX("../DLL/Town/Street environment_V01.fbx");
	//App->fbxImporter->loadFBX("../DLL/Warrior.FBX");
	//id = App->fbxImporter->loadTextures();
	//id = App->texture->loadTexture("../../lena.png");


	App->goManager->LoadFBX("root/data/assets/meshes/Street environment_V01.fbx");
	//App->goManager->LoadFBX("root/data/assets/meshes/MechaT.FBX");

	//GameObject* go = App->goManager->root;

	return true;
}
update_status TestScene1::PreUpdate(float dt)
{ 

	return UPDATE_CONTINUE;
}
update_status TestScene1::Update(float dt)
{ 

	return UPDATE_CONTINUE;
}
update_status TestScene1::PostUpdate(float dt)
{
	//GRID
	P_Plane grid(0.f, 1.f, 0.f, 0.f);
	grid.axis = true;
	//grid.wire = true;
	grid.color.Set(255, 255, 255);
	grid.Render();
	return UPDATE_CONTINUE;
}
bool TestScene1::CleanUp()
{
	return true;
}