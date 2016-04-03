#include "stdafx.h"
#include "Workspace.h"
#include <glm\gtx\vector_angle.hpp>

Workspace::Workspace(int _width, int _height)
{
	width = _width; height = _height;
	FoV = 45.0f;
	mouseSpeed = 0.0005f;
	pov = vec3(0, 2, 10);
	//angleFree = vec2(3.14, -0.698); //horizontal-vertical
	direction = vec3(0, 0, -1);
	right = vec3(1, 0, 0);
	up = normalize(vec3(0, 1, 0));
	recView();
	recProj(width, height);
}


Workspace::~Workspace()
{
}

void Workspace::rotateCamera(POINT shift)
{
	if (center)
		rotatePoV(shift);
	else
		rotateDirection(shift);
}

void Workspace::rotatePoV(POINT shift)
{
	int mouse_x = shift.x;
	int mouse_y = shift.y;

	
	pov = rotateX(vec3(pov - *center), mouseSpeed*mouse_x);
	pov = rotateY<float, highp>(pov, mouseSpeed*mouse_y) + *center;
	direction = normalize(pov - *center);
	up = rotateX(up, -mouseSpeed*mouse_x);
	up = rotateY(up, -mouseSpeed*mouse_x);
	right = rotateX(right, -mouseSpeed*mouse_x);
	right = rotateY(right, -mouseSpeed*mouse_x);

	recView();
}

void Workspace::rotateDirection(POINT shift)
{
	int mouse_x = shift.x;
	int mouse_y = shift.y;


	direction = rotateX(direction, mouseSpeed*mouse_x);
	direction = rotateY(direction, mouseSpeed*mouse_y);

	up = rotateX(up, mouseSpeed*mouse_x);
	up = rotateY(up, mouseSpeed*mouse_y);

	right = rotateX(right, mouseSpeed*mouse_x);
	right = rotateY(right, mouseSpeed*mouse_y);

	recView();
}

void Workspace::shiftCamera(POINT shift)
{
	POINT fake; fake.x = width; fake.y = height/2;
	vec3 normal = cross(up, right);
	float d = -dot(normal, selected->getPos());
	float dD = d + dot(normal, pov);

	vec3 right = rayCalc(fake);
	float l = abs(dD / dot(normal,right));
	right = right*l + pov;
	
	fake.x = 0;
	vec3 left = rayCalc(fake);
	l = abs(dD / dot(normal, left));
	left = left*l + pov;

	fake.x = width / 2; fake.y = 0;
	vec3 bottom = rayCalc(fake);
	l = abs(dD / dot(normal, bottom));
	bottom = bottom*l + pov;

	fake.y = height;
	vec3 top = rayCalc(fake);
	l = abs(dD / dot(normal, top));
	top = top*l + pov;

	vec3 dX = (right - left) / (float)width;
	vec3 dY = (top - bottom) / (float)height;
	pov += dX*(float)shift.x + dY*(float)shift.y;
	recView();
}

void Workspace::shiftSelected(POINT shift)
{
	POINT fake; fake.x = width; fake.y = height / 2;
	vec3 normal = cross(up, right);
	float d = -dot(normal, selected->getPos());
	float dD = d + dot(normal, pov);

	vec3 right = rayCalc(fake);
	float l = abs(dD / dot(normal, right));
	right = right*l + pov;

	fake.x = 0;
	vec3 left = rayCalc(fake);
	l = abs(dD / dot(normal, left));
	left = left*l + pov;

	fake.x = width / 2; fake.y = 0;
	vec3 bottom = rayCalc(fake);
	l = abs(dD / dot(normal, bottom));
	bottom = bottom*l + pov;

	fake.y = height;
	vec3 top = rayCalc(fake);
	l = abs(dD / dot(normal, top));
	top = top*l + pov;
	
	vec3 dX = (right - left) / (float)width;
	vec3 dY = (top - bottom) / (float)height;
	selected->move(dX*(float)shift.x + dY*(float)shift.y);
	//recView();
}

void Workspace::recProj(int x, int y)
{
	width = x;
	height = y;
	proj = perspective(radians(FoV), (float)width/(float)height , 0.1f, 100.0f);
}

void Workspace::recView()
{
	view = lookAt(pov, pov + direction, up);
}

vec3 Workspace::rayCalc(POINT& clickPoint)
{
	int mouse_x = clickPoint.x;
	int mouse_y = clickPoint.y;

	float x = (2.0f * mouse_x) / width - 1.0f;
	float y = 1.0f - (2.0f * mouse_y) / height;
	float z = 1.0f;
	vec4 ray_clip = vec4(x, y, -1.0, 1.0);
	vec4 ray_eye = inverse(proj) * ray_clip;
	ray_eye = vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
	vec3 ray_wor = vec3((inverse(view) * ray_eye));
	ray_wor = normalize(ray_wor);


	return ray_wor;
}

void Workspace::freeMove(POINT shift)
{
}

void Workspace::changeColor(vec4 newcolor)
{
	if (selected)
		selected->setColor(newcolor);
}
//Дальше идет костыль. Ибо этот момент даже в 3д редакторах не совсем адекватно реализован -_-
void Workspace::axisMove(UINT axis, POINT shift)//0=x, 1=y, 2=z
{
	POINT fake; fake.x = width; fake.y = height / 2;
	vec3 normal = cross(up, right);
	float d = -dot(normal, selected->getPos());
	float dD = d + dot(normal, pov);

	vec3 right = rayCalc(fake);
	float l = abs(dD / dot(normal, right));
	right = right*l + pov;

	fake.x = 0;
	vec3 left = rayCalc(fake);
	l = abs(dD / dot(normal, left));
	left = left*l + pov;

	fake.x = width / 2; fake.y = 0;
	vec3 bottom = rayCalc(fake);
	l = abs(dD / dot(normal, bottom));
	bottom = bottom*l + pov;

	fake.y = height;
	vec3 top = rayCalc(fake);
	l = abs(dD / dot(normal, top));
	top = top*l + pov;

	vec3 dX = (right - left) / (float)width;
	vec3 dY = (top - bottom) / (float)height;
	vec3 toMove = dX*(float)shift.x + dY*(float)shift.y;
	switch (axis)
	{
	case 0:
		toMove.y = 0; toMove.z = 0;
		break;
	case 1:
		toMove.x = 0; toMove.z = 0;
		break;
	case 2:
		toMove.y = 0; toMove.x = 0;
		break;
	default:
		break;
	}
	selected->move(toMove);
}

void Workspace::setSelected(CSceneObject *model)
{
	selected = model;
	setCenter(selected->getPos());
}

void Workspace::remSelected()
{
	selected = nullptr;
}

void Workspace::setCenter(vec3 a)
{
	center = new vec3(a);
}

void Workspace::remCenter()
{
	delete center;
}

void Workspace::changeDistance(float shift)
{
	pov -= shift*direction;
}

void Workspace::scale(POINT shift, POINT curr)
{
}

void Workspace::rotate(POINT shift)
{
	int mouse_x = shift.x;
	int mouse_y = shift.y;

	selected->rotate(vec3(mouse_x*mouseSpeed, mouse_y*mouseSpeed, 0));

}
