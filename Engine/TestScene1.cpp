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
	App->fbxImporter->loadFBX("../DLL/MechaT.FBX");
	//App->fbxImporter->loadFBX("../DLL/Warrior.FBX");
	id = App->fbxImporter->loadTextures();
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


	


	////Draw a quad in direct mode
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, id);
	glBegin(GL_TRIANGLES);

	float s = 1.f * 2;

	//----------------------- Front
	
	glTexCoord2f(1.f, 1.f);
	glVertex3d(s, s, s);	
	glTexCoord2f(0.f, 0.f);
	glVertex3d(-s, -s, s);	
	glTexCoord2f(0.f, 1.f);
	glVertex3d(s, -s, s);

	glTexCoord2f(1.f, 0.f);
	glVertex3d(-s, s, s);	
	glTexCoord2f(0.f, 0.f);
	glVertex3d(-s, -s, s);	
	glTexCoord2f(1.f, 1.f);
	glVertex3d(s, s, s);
	//----------------------- Top
	glVertex3d(-s, s, s);
	glVertex3d(s, s, s);
	glVertex3d(s, s, -s);

	glVertex3d(-s, s, s);
	glVertex3d(s, s, -s);
	glVertex3d(-s, s, -s);
	//----------------------- Right
	glVertex3d(s, s, s);
	glVertex3d(s, -s, s);
	glVertex3d(s, s, -s);

	glVertex3d(s, -s, s);
	glVertex3d(s, -s, -s);
	glVertex3d(s, s, -s);
	//----------------------- Left
	glVertex3d(-s, s, -s);
	glVertex3d(-s, -s, s);
	glVertex3d(-s, s, s);

	glVertex3d(-s, s, -s);
	glVertex3d(-s, -s, -s);
	glVertex3d(-s, -s, s);
	//----------------------- Bottom
	glVertex3d(-s, -s, s);
	glVertex3d(s, -s, -s);
	glVertex3d(s, -s, s);

	glVertex3d(-s, -s, s);
	glVertex3d(-s, -s, -s);
	glVertex3d(s, -s, -s);
	//----------------------- Behind
	glVertex3d(s, s, -s);
	glVertex3d(s, -s, -s);
	glVertex3d(-s, s, -s);

	glVertex3d(s, -s, -s);
	glVertex3d(-s, -s, -s);
	glVertex3d(-s, s, -s);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
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