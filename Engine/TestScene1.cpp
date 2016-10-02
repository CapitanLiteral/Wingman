#include "Application.h"
#include "Globals.h"
#include "TestScene1.h"
#include "ModuleFBXImporter.h"



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
	App->fbxImporter->loadFBX("C:/Users/anara/Documents/GitHub/Wingman/Engine/Assimp/warrior.fbx");
	return true;
}
update_status TestScene1::PreUpdate(float dt)
{ 

	return UPDATE_CONTINUE;
}
update_status TestScene1::Update(float dt)
{ 

	////Drawing a line in direct mode
	//glLineWidth(2.0f);

	//glBegin(GL_LINES);
	//glVertex3f(0.f, 0.f, 0.f);
	//glVertex3f(0.f, 10.f, 0.f);
	//glEnd();
	//glLineWidth(1.0f);

	////Draw a quad in direct mode
	//glBegin(GL_QUADS);
	//glVertex3f(0.f, 0.f, 0.f);
	//glVertex3f(0.f, 10.f, 0.f);
	//glVertex3f(-10.0f, 10.f, 0.f);
	//glVertex3f(-10.0f, 0.f, 0.f);
	//glEnd();

	App->fbxImporter->drawMeshes(App->fbxImporter->meshes);
	
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