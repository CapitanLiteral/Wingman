#include "Application.h"
#include "ComponentCamera.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "OpenGL.h"
#include "Color.h"

ComponentCamera::ComponentCamera(GameObject* parent) : Component(parent)
{	
	frustum = new Frustum();
	frustum->type = PerspectiveFrustum;
	frustum->up = float3(0,1,0);
	frustum->front = float3(1, 0, 0);
	frustum->pos = float3(0,0,0);
	frustum->verticalFov = 75 * DEGTORAD;
	//frustum->horizontalFov = 2.f * atan(tan(frustum->verticalFov * 0.5f) * aspectRatio);
	frustum->horizontalFov = 2.f * atan(tan(frustum->verticalFov * 0.5f) * 16/9);
	frustum->farPlaneDistance = 100.f;
	frustum->nearPlaneDistance = 10.f;
	

	
}


ComponentCamera::~ComponentCamera()
{
	//RELEASE(frustum);
}

void ComponentCamera::update()
{
	if (debug)
	{
		draw();
	}
}

void ComponentCamera::draw()
{
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glLineWidth(1.f);
	Color obb_color;
	obb_color.Set(1, 0, 0, 1);
	glColor4f(obb_color.r, obb_color.g, obb_color.b, obb_color.a);

	float3 vertices[8];
	frustum->GetCornerPoints(vertices);

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
