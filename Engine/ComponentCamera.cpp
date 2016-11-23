#include "Application.h"
#include "ComponentCamera.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "GameObject.h"
#include "OpenGL.h"
#include "Color.h"
#include "ModuleCamera3D.h"

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
	RELEASE(frustum);
}

void ComponentCamera::update()
{
	float3 pos;
	float3 scale;
	Quat rot;
	parent->globalTransform.Decompose(pos,rot,scale);
	//http://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
	frustum->pos = pos;	
	frustum->up = rot.WorldY();
	frustum->front = rot.WorldX();
	//frustum->front = rot * frustum->front * rot.Conjugated();
	if (debug)
	{
		draw();
	}

	if (attachCamera)
	{
		App->camera->Position.x = pos.x;
		App->camera->Position.y = pos.y;
		App->camera->Position.z = pos.z;

		App->camera->LookAt(frustum->CenterPoint());

	}

}

//void rotate_vector_by_quaternion(const float3& v, const Quat& q, float3& vprime)
//{
//	// Extract the vector part of the quaternion
//	float3 u(q.x, q.y, q.z);
//
//	// Extract the scalar part of the quaternion
//	float s = q.w;
//
//	// Do the math
//	vprime = 2.0f * dot(u, v) * u
//		+ (s*s - dot(u, u)) * v
//		+ 2.0f * s * cross(u, v);
//}

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
