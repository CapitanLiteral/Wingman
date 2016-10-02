#include "Application.h"
#include "Globals.h"
#include "TestScene1.h"
#include "ModuleFBXImporter.h"


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
	App->fbxImporter->loadFBX("C:/Users/anara/Documents/GitHub/Wingman/Engine/Assimp/warrior.fbx");
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

	return UPDATE_CONTINUE;
}
bool TestScene1::CleanUp()
{
	return true;
}