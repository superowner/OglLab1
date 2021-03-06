#include "stdafx.h"
#include "SceneObject.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/gtx/rotate_vector.hpp>
using namespace glm;
#include <glm/gtx/transform.hpp>
#ifdef _DEBUG

#include <iostream>
using namespace std;
//#define CONSOLE_DEBUG

#endif
#define EPSILON 0.000001
bool floatEquality(float &a, float &b)
{
	return (a - EPSILON<b + EPSILON&& a + EPSILON>b - EPSILON);
}
//
//bool isInsideTriangle(const vec3& pt, const vec3& v0, const vec3& v1, const vec3& v2, const vec3& normal)
//{
//	short tmp = 0;
//	if (dot(cross(v1 - v0, pt - v0), normal) < EPSILON)
//		tmp++;
//	else
//		tmp--;
//	if (dot(cross(v2 - v1, pt - v1), normal)<EPSILON)
//		tmp++;
//	else
//		tmp--;	
//	if (dot(cross(v2 - v1, pt - v1), normal)<EPSILON)
//		tmp++;
//	else
//		tmp--;	
//	return (tmp==3||tmp==-3);
//}
float triangle_square(float a, float b, float c) {
	float p = (a + b + c) / 2;
	return sqrt(p*(p - a)*(p - b)*(p - c));
}
bool inside_triangle(vec3 P, vec3 A, vec3 B, vec3 C) 
{
	float AB = distance(A, B);
	float BC = distance(B, C);
	float CA = distance(C, A);

	float AP = distance(A, P);
	float BP = distance(B, P);
	float CP = distance(C, P);
	float diff = (triangle_square(AP, BP, AB) + triangle_square(AP, CP, CA) + triangle_square(BP, CP, BC)) - triangle_square(AB, BC, CA);
	if (fabs(diff)<EPSILON) return true;
	return false;
}

CSceneObject::CSceneObject(GLuint vao=0, vec3 pos = vec3(0), vec3 rot = vec3(0), vec3 scal = vec3(1))
{
	position = pos;
	rotation = rot;
	scale = scal;
	selectionMode = false;
	drawProjection = 0;
	colour = vec4(1);
	rotateM = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
}


CSceneObject::~CSceneObject()
{	
}

void CSceneObject::draw(GLuint a,GLuint k, GLuint b , GLuint c, mat4* d, GLuint e)
{
}

void CSceneObject::setScale(vec3 _scale)
{
	scale = _scale;
}

void CSceneObject::move(glm::vec3 _pos)
{
	position += _pos;
}

void CSceneObject::translate(vec3 _pos)
{
	position =_pos;
}

void CSceneObject::rotate(vec3)
{
}

void CSceneObject::rotate(glm::mat4)
{
}

void CSceneObject::projection(GLushort d)
{
	drawProjection = d;
}

void CSceneObject::setColor(vec4 a)
{
	colour = a;
}

vec3 CSceneObject::getScale()
{
	return scale;
}

vec3 CSceneObject::getPos()
{
	return position;
}

vec3 CSceneObject::getRotation()
{
	return rotation;
}

glm::vec4 CSceneObject::getColor()
{
	return colour;
}


GLboolean CSceneObject::selectionRayTry(vec3, vec3, vec3 &)
{
	return GLboolean();
}



void CPolygonalPrismObject::draw(GLuint vao, GLuint translateID,GLuint colorUniformID, GLuint mvUniformID,mat4* view, GLuint enableDirectLightID)
{
	mat4 t = glm::translate(position);
	mat4 mv = (*view)*t*rotateM*mat4(1.0);
	bool enable = 1;
	vec3 pos = vec3(0);
	glUniform1i(enableDirectLightID, enable);
	glUniform3fv(translateID, 1, &pos[0]);
	glUniformMatrix4fv(mvUniformID, 1, GL_FALSE, &mv[0][0]);
	
		glUniform4fv(colorUniformID, 1, &colour[0]);
	
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glDrawArrays(GL_TRIANGLES,0, vertices.size());

	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	if (selectionMode)
	{
		bool enable = 0;
		glUniform1i(enableDirectLightID, enable);
		vec4 selectionCol = vec4(1, 0.645f, 0, 1);
		glUniform4fv(colorUniformID, 1, &selectionCol[0]);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

		//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
	if (drawProjection)
	{
		enable = 0;
		glUniform1i(enableDirectLightID, enable);
		vec3 project = vec3(0);
		switch (drawProjection)
		{
		case 1:
			mv = (*view)*t*rotateM*mat4 (
				vec4(1,0,0, 0),
				vec4(0,1,0, 0),
				vec4(0,0,0, 0),
				vec4(0,0,0, 1)
			);
			project.z = 0;
			break;
		case 2:
			mv = (*view)*t*rotateM*mat4 (
				vec4(0,0,0, 0),
				vec4(0,1,0, 0),
				vec4(0,0,1, 0),
				vec4(0,0,0, 1)
			);
			project.x = 0;
			break;
		case 3:
			mv = (*view)*t*rotateM*mat4 (
				vec4(1,0,0, 0),
				vec4(0,0,0, 0),
				vec4(0,0,1, 0),
				vec4(0,0,0, 1)
			);
			project.y = 0;
			break;
		default:
			mv = (*view)*t*rotateM*mat4 (
				vec4(1,0,0, 0),
				vec4(0,1,0, 0),
				vec4(0,0,1, 0),
				vec4(0,0,0, 1)
			);
			break;

		}
		glUniformMatrix4fv(mvUniformID, 1, GL_FALSE, &mv[0][0]);
		glUniform3fv(translateID, 1, &project[0]);

		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

	}

}

void CPolygonalPrismObject::setScale(vec3 a)
{
	//CSceneObject::setScale(a);
	mat3 scaleMatrix = mat3
	{
		vec3(a.x / scale.x, 0, 0),
		vec3(0,a.y / scale.y, 0),
		vec3(0,0,a.z / scale.z)
	};
	CSceneObject::setScale(a);
	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i] = scaleMatrix*vertices[i];
	}
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec3), &vertices[0], GL_STATIC_DRAW);

}

void fixAngle(vec3 &rot)
{
	while (rot.x > 2*M_PI)
		rot.x -= 2*M_PI;
	while (rot.x < -2*M_PI)
		rot.x += 2*M_PI;
	while (rot.y > 2*M_PI)
		rot.y -= 2*M_PI;
	while (rot.y < -2*M_PI)
		rot.y += 2*M_PI;
	while (rot.z > 2*M_PI)
		rot.z -= 2*M_PI;
	while (rot.z < -2*M_PI)
		rot.z += 2*M_PI;
}

void CPolygonalPrismObject::rotate(vec3 rot)
{
	rotation += rot;
	fixAngle(rotation);
	//if (rot != vec3(0))
	//{
	//	for (short i = 0; i < vertices.size(); i++)
	//	{
	//		//rotateX(vertices[i], rot.x);
	//		vertices[i] = glm::rotate<float, highp>(vertices[i], rot.x, vec3(1, 0, 0));
	//		vertices[i] = glm::rotate<float, highp>(vertices[i], rot.y, vec3(0, 1, 0));
	//		vertices[i] = glm::rotate<float, highp>(vertices[i], rot.z, vec3(0, 0, -1));
	//		normals[i] = glm::rotate<float, highp>(normals[i], rot.x, vec3(1, 0, 0));
	//		normals[i] = glm::rotate<float, highp>(normals[i], rot.y, vec3(0, 1, 0));
	//		normals[i] = glm::rotate<float, highp>(normals[i], rot.z, vec3(0, 0, -1));
	//	}
	//}
	//glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec3), &vertices[0], GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	//glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(vec3), &normals[0], GL_STATIC_DRAW);
		rotateM = glm::rotate(rotation.x, vec3(1, 0, 0))*glm::rotate(rotation.y, vec3(0, 1, 0))*glm::rotate(rotation.z, vec3(0, 0, -1));

}

void CPolygonalPrismObject::rotate(glm::mat4 a)
{
	rotateM = a*rotateM;
}


vec3 findNormal(vec3 coor2, vec3 coor1, vec3 coor0)
{
	vec3 a = vec3(coor1.x - coor0.x, coor1.y - coor0.y, coor1.z - coor0.z);
	vec3 b = vec3(coor2.x - coor0.x, coor2.y - coor0.y, coor2.z - coor0.z);

	return vec3(
		a.y * b.z - b.y * a.z,
		a.z * b.x - a.x * b.z,
		a.x * b.y - b.x * a.y
		);
}
glm::vec3 computeNormal
(
	glm::vec3 const & a,
	glm::vec3 const & b,
	glm::vec3 const & c
	)
{
	return glm::normalize(glm::cross(c - a, b - a));
}


CPolygonalPrismObject::CPolygonalPrismObject(GLuint vao=0, vec3 pos=vec3(0), vec3 rot = vec3(0), vec3 _scale = vec3(1), GLuint sidesNum=3, GLfloat _height=1, GLfloat _width=1, vec4 _colour=vec4(0))
{
	//CSceneObject(vao, pos, rot, _scale);
	position = pos;
	rotation = rot;
	scale = _scale;
	selectionMode = false;
	drawProjection = 0;
	colour = _colour;
	sides = sidesNum;
	height = _height; width = _width;
	GLfloat R = width*sides / (2 * M_PI);
	std::vector<vec3> tmp_vertices;
	tmp_vertices.push_back(vec3(R, -height/2, 0));
	for (int i = 1; i < sides; i++)
	{
			tmp_vertices.push_back(glm::rotate<float, highp>(tmp_vertices[i - 1], (float)2 * M_PI / sides, vec3(0, 1, 0)));
	}
	tmp_vertices.insert(tmp_vertices.end(), tmp_vertices.begin(), tmp_vertices.end());
	for (short i = sides; i < 2 * sides; i++)
		tmp_vertices[i].y = -tmp_vertices[i].y;
	//normals.insert(normals.begin(), vertices.begin(), vertices.end());
	//for (int i = 0; i < sides; i++)
	//{
	//	normals[i] = normalize(normals[i]);

	//	//normals[i].y = 0;
	//	//normals[i]=normalize(normals[i]);
	//	//normals[i] = normalize(vec3(0, vertices[i].y, 0) + normals[i]);
	//}
	//
	if (scale != vec3(1))
	{
		for (short i = 0; i < 2 * sides; i++)
		{
			tmp_vertices[i].x *= scale.x;
			tmp_vertices[i].y *= scale.y;
			tmp_vertices[i].z *= scale.z;
		}
	}
	vertices.resize(12 * sides - 12);
	normals.resize(12 * sides - 12);
	for (int i = 0; i < sides - 2; i++)
	{
		vertices[3 * i] = tmp_vertices[0];
		vertices[3 * i + 1] = tmp_vertices[i + 1];
		vertices[3 * i + 2] = tmp_vertices[i + 2];
		vertices[9 * sides - 6 + 3 * i] = tmp_vertices[sides];
		vertices[9 * sides - 5 + 3 * i] = tmp_vertices[sides + i + 1];
		vertices[9 * sides - 4 + 3 * i] = tmp_vertices[sides + (i + 2) % (sides)];
	}
	for (int i = 0; i < sides; i++)
	{
		vertices[3 * (sides - 2) + 6 * i] = tmp_vertices[i];
		vertices[3 * (sides - 2) + 6 * i + 1] = tmp_vertices[(i + 1)%sides];
		vertices[3 * (sides - 2) + 6 * i + 2] = tmp_vertices[sides + i];
		vertices[3 * (sides - 2) + 6 * i + 3] = tmp_vertices[sides + i];
		vertices[3 * (sides - 2) + 6 * i + 4] = tmp_vertices[(i + 1)%sides];
		vertices[3 * (sides - 2) + 6 * i + 5] = tmp_vertices[sides + (i + 1)%(sides)];
	}
	tmp_vertices.clear();
	for (int i = 0; i < vertices.size() / 3; i++)
	{
		normals[3 * i] = normalize(computeNormal(vertices[3 * i], vertices[3 * i + 1], vertices[3 * i + 2]));
		normals[3 * i + 1] = normals[3 * i];
		normals[3 * i + 2] = normals[3 * i];

	}
	for (short i = 0; i <  vertices.size(); i++)
	{
		vertices[i] = glm::rotate<float, highp>(vertices[i], (float)M_PI / sides, vec3(0, 1, 0));
		normals[i] = glm::rotate<float, highp>(normals[i], (float)M_PI / sides, vec3(0, 1, 0));
	}

		/*for (short i = 0; i < vertices.size(); i++)
		{
			vertices[i] = glm::rotate<float, highp>(vertices[i], rot.x, vec3(1, 0, 0));
			vertices[i] = glm::rotate<float, highp>(vertices[i], rot.y, vec3(0, 1, 0));
			vertices[i] = glm::rotate<float, highp>(vertices[i], rot.z, vec3(0, 0, -1));
			normals[i] = glm::rotate<float, highp>(normals[i], rot.x, vec3(1, 0, 0));
			normals[i] = glm::rotate<float, highp>(normals[i], rot.y, vec3(0, 1, 0));
			normals[i] = glm::rotate<float, highp>(normals[i], rot.z, vec3(0, 0, -1));
		}*/
	rotateM = glm::rotate(rot.x, vec3(1, 0, 0))*glm::rotate(rot.y, vec3(0, 1, 0))*glm::rotate(rot.z, vec3(0, 0, -1));
	glBindVertexArray(vao);
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec3), &vertices[0], GL_STATIC_DRAW);
	glGenBuffers(1, &normalsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(vec3), &normals[0], GL_STATIC_DRAW);

}

//CPolygonalPrismObject::CPolygonalPrismObject(GLuint vao, glm::vec3 pos, glm::vec3 rot, glm::vec3 _scale, GLuint sidesNum, GLfloat height_, GLfloat width_, glm::vec4 _colour, mat4 rotation_)
//{
//	
//	
//	rotateM *= rotation_;
//
//}
CPolygonalPrismObject::~CPolygonalPrismObject()
{
	vertices.clear();
	indices.clear();
	normals.clear();
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &elementBuffer);
	glDeleteBuffers(1, &normalsBuffer);

}
GLboolean CPolygonalPrismObject::selectionRayTry(vec3 pickingRay, vec3 observingPoint, vec3 &intersectionPoint)
{
	vec4 plane, tmp_plane;
	bool get = false;
	vec3 intersection1, intersection2;
	for (int i = 0; i < vertices.size() / 3; i++)
	{
		vec3 vertices_[3];
		vertices_[0] = vertices[3 * i] + position;
		vertices_[1] = vertices[3 * i + 1] + position;
		vertices_[2] = vertices[3 * i + 2] + position;
		vec3 n = normals[3 * i];

		float D = -dot(n,vertices_[0]);
		float dD = D + dot(n, observingPoint);
		float l = abs(dD / (n.x*pickingRay.x + n.y*pickingRay.y + n.z*pickingRay.z));

		if (l == 0) continue;
		vec3 plane_intersection = pickingRay*l+observingPoint;
		if (fabs(dot(plane_intersection, n) + D) > EPSILON)
			continue;

		if (inside_triangle(plane_intersection, vertices_[0], vertices_[1], vertices_[2])){
			if (get){
				intersection2 = plane_intersection;
				break;
			}
			else{
				intersection1 = plane_intersection;
				get = true;
			}
		}
	}
	
	if (get){
		if (distance(intersection1, observingPoint) > distance(intersection2, observingPoint))
			intersectionPoint = intersection2;
		else
			intersectionPoint = intersection1;
		return true;
	}
	else
		return false;
}
