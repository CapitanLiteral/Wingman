#include "Application.h"
#include "ComponentCamera.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "GameObject.h"
#include "OpenGL.h"
#include "Color.h"
#include "ModuleCamera3D.h"
#include "ModuleFileSystem.h"
#include "JsonSerializer.h"

ComponentCamera::ComponentCamera(GameObject* parent) : Component(parent)
{	
	frustum = new Frustum();
	

	JsonSerializer::Deserialize(this, "root/data/library/cameras/camera.json");

	//std::string output;
	//JsonSerializer::Serialize(this, output, "data/library/cameras/camera.json");

}


ComponentCamera::~ComponentCamera()
{
	RELEASE(frustum);
}

void ComponentCamera::Serialize(Json::Value & root)
{	
	LCG random;
	UUID = random.Int();
	std::string uuid_str = std::to_string(UUID);	
	root["UUID"] = UUID;
	root["component_type"] = "camera";
	//Type
	root["type"] = frustum->type;
	//Fov
	root["vertical_fov"] = frustum->verticalFov * RADTODEG;
	//Far plane
	root["far_plane"] = frustum->farPlaneDistance;
	//Near plane
	root["near_plane"] = frustum->nearPlaneDistance;

}

void ComponentCamera::Deserialize(Json::Value & root)
{
	////Position
	//Json::Value position = root.get("pos", 0);
	//std::vector<Json::Value> results;
	//for (int i = 0; i != position.size(); i++)
	//		*(frustum->pos.ptr()+i) = position[i].asFloat();
	//Type
	frustum->type = (FrustumType)root.get("type", 2).asInt();
	////Up
	//Json::Value up = root.get("pos", 0);
	//for (int i = 0; i != up.size(); i++)
	//	*(frustum->up.ptr() + i) = up[i].asFloat();
	////Front
	//Json::Value front = root.get("pos", 0);
	//for (int i = 0; i != front.size(); i++)
	//	*(frustum->front.ptr() + i) = front[i].asFloat();
	//Fov
	frustum->verticalFov = root.get("vertical_fov", 75).asFloat() * DEGTORAD;
	frustum->horizontalFov = 2.f * atan(tan(frustum->verticalFov * 0.5f) * 16 / 9);
	//Far Plane
	frustum->farPlaneDistance = root.get("far_plane", 100).asFloat();
	//Near Plane
	frustum->nearPlaneDistance = root.get("near_plane", 10).asFloat();

}

void ComponentCamera::Update()
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

	//TODO: We need to have more than one camera...
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
