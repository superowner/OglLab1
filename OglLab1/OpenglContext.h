#pragma once

#include "SceneObject.h"
#include <vector>

using namespace std;
using namespace glm;
class COpenglContext
{
public:
	std::vector<CSceneObject*> fieldObjects;
private:
	vec3 pov;
	vec2 angleFree; //horizontal-vertical for free camera
	float FoV;
	float mouseSpeed;
	vector <GLushort> selectedObjects;

	
	GLuint width, height;

	vec3 direction, right, up;

	GLuint vao;
	GLuint ColourID;
	GLuint MatrixID_MV;
	GLuint MatrixID_Projection;
	//GLuint MatrixID_Normal;
	GLuint translateID;
	GLuint LightVecAmbientID;
	GLuint LightPosID;
	GLuint LightVecColorID;

	GLuint LightShineID;
	GLuint LightStrengthID;

	vec3 lAmbient;
	vec3 lPos;
	vec3 lightColor;

	mat4 proj;
	mat4 view;
	

	GLfloat ambientCoef;

	GLfloat lShine;
	GLfloat lStrength;

	GLuint enableDirectLightID;

	GLuint prog;
public:
	void Init();
	void Draw();
	void resize(int x, int y);
	void addObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, GLuint sidesNum, GLfloat height, GLfloat width, glm::vec4 color);
	void moveSelected();
	COpenglContext();
	~COpenglContext();
};

