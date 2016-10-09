#include "Application.h"
#include "Globals.h"
#include "TestScene2.h"
#include "ModuleFBXImporter.h"

#include "OpenGL.h"

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "Glew/libx86/glew32.lib") /* link Microsoft OpenGL lib   */

TestScene2::TestScene2(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


TestScene2::~TestScene2()
{
}

bool TestScene2::Init()
{
	return true;
}
bool TestScene2::Start()
{
	return true;
}
update_status TestScene2::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}
update_status TestScene2::Update(float dt)
{

	return UPDATE_CONTINUE;
}
update_status TestScene2::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}
bool TestScene2::CleanUp()
{
	return true;
}