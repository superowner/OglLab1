#pragma once

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#ifdef _DEBUG

#pragma comment(lib, "glew32d.lib")
//#pragma comment (lib, "freeglut.lib")
#else
#pragma comment(lib, "glew32.lib")
#endif

#include <GL/glew.h>
#include <GL/wglew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <glm\gtc\matrix_transform.hpp>




class CSceneObject
{
protected:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::mat4 rotateM;
	GLboolean selectionMode; 
	GLushort drawProjection; //1 - x0y, 2 - y0z, 3 - x0z
	glm::vec3 scale;
	glm::vec4 colour;
	//0 - not selected; 1 - is selected. 
	//if selected, draw lines&points increased of in certain colour
public:
	virtual void draw(GLuint, GLuint, GLuint,  GLuint, glm::mat4*, GLuint);
	virtual void setScale(glm::vec3);
	virtual void move(glm::vec3);
	virtual void translate(glm::vec3);
	virtual void rotate(glm::vec3);
	virtual void rotate(glm::mat4);
	virtual void projection(GLushort); //1 - x0y, 2 - y0z, 3 - x0z
	virtual void setColor(glm::vec4);
	virtual int getNum() { return 0; }
	virtual float getWidth() { return 0; }
	virtual float getHeight() { return 0; }
	virtual float getRotM(int i, int j) { return rotateM[i][j]; }
	virtual glm::vec3 getScale();
	virtual glm::vec3 getPos();
	virtual glm::vec3 getRotation();
	virtual glm::vec4 getColor();
	virtual GLboolean selectionRayTry(glm::vec3, glm::vec3, glm::vec3&);

	CSceneObject(GLuint, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal);
	~CSceneObject();
	extern friend class COpenglContext;
};

class CPolygonalPrismObject : public CSceneObject
{
private:
	GLuint vertexBuffer;
	GLuint normalsBuffer;
	GLuint elementBuffer;
	GLuint sides;
	std::vector<glm::vec3> vertices; //size=2n;
	
	std::vector<glm::vec3> normals;
	std::vector<unsigned short> indices;
	GLfloat height, width; //height of sides, width of
public:
	void draw(GLuint, GLuint,GLuint, GLuint, glm::mat4*, GLuint);
	void setScale(glm::vec3);
	void rotate(glm::vec3);
	void rotate(glm::mat4);
	GLboolean selectionRayTry(glm::vec3, glm::vec3, glm::vec3&);
	int getNum() { return sides; }
	float getWidth() { return width; }
	float getHeight() { return height; }
	//GLuint translateID;
	CPolygonalPrismObject(GLuint, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale , GLuint sidesNum, GLfloat height, GLfloat width, glm::vec4 color);
	CPolygonalPrismObject(GLuint vao, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, GLuint sidesNum, GLfloat height, GLfloat width, glm::vec4 color, glm::mat4 rotation) :CPolygonalPrismObject(vao, pos, rot, scale, sidesNum, height, width, color) { rotateM = rotation; }
	~CPolygonalPrismObject();
	extern friend class COpenglContext;

};