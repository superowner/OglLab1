#pragma once
#include "stdafx.h"
#include "SceneObject.h"
#include <glm/glm.hpp>
using namespace glm;
class Workspace
{
private:
	vec3 pov;
	vec3* center;
	
	float FoV;
	float mouseSpeed;
	
	vec3 direction, right, up;
	
	CSceneObject* selected;

	mat4 proj;
	mat4 view;
	GLuint width, height;
public:
	
	Workspace(int x=800,int y=600);
	//Workspace() :Workspace() {}
	~Workspace();
	void rotateCamera(POINT shift);
	void rotatePoV(POINT shift);
	void rotateDirection(POINT shift);
	void shiftCamera(POINT shift);
	void shiftSelected(POINT shift);
	void recProj(int,int);
	void recView();
	vec3 rayCalc(POINT&);
	void freeMove(POINT shift);
	void changeColor(vec4 newcolor);
	void shiftColor(POINT shift);
	void axisMove(UINT axis, POINT shift);
	void setPoV(vec3 a) { pov = a; }
	void setDirection(vec3 dir) { direction = dir; }
	void setSelected(CSceneObject*);
	void remSelected();
	void setCenter(vec3);
	void remCenter();

	void changeDistance(float shift);

	void scale(POINT mouseShift);
	void rotate(POINT mouseShift);


	extern friend class COpenglContext;
};

