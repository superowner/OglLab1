#include "stdafx.h"
#include "SceneObject.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/gtx/rotate_vector.hpp>
using namespace glm;
CSceneObject::CSceneObject(GLuint vao=0, vec3 pos = vec3(0), vec3 rot = vec3(0), vec3 scal = vec3(1))
{
	position = pos;
	rotation = rot;
	scale = scal;
	selectionMode = false;
	drawProjection = 0;
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

void CSceneObject::translate(vec3 _pos)
{
	position +=_pos;
}

void CSceneObject::rotate(vec3)
{
}

void CSceneObject::projection(GLushort d)
{
	drawProjection = d;
}


GLboolean CSceneObject::selectionRayTry(vec3, vec3, vec3 &)
{
	return GLboolean();
}



void CPolygonalPrismObject::draw(GLuint vao, GLuint translateID,GLuint colorUniformID, GLuint mvUniformID,mat4* view, GLuint enableDirectLightID)
{
	mat4 mv = (*view)*mat4(1.0);
	bool enable = 1;
	glUniform1i(enableDirectLightID, enable);
	glUniform3fv(translateID, 1, &position[0]);
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
		vec4 selectionCol = vec4(0xff / 0x100, 0xa5 / 0x100, 0x00, 1);
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
		vec3 project = position;
		switch (drawProjection)
		{
		case 1:
			mv = (*view)*mat4 (
				vec4(1,0,0, 0),
				vec4(0,1,0, 0),
				vec4(0,0,0, 0),
				vec4(0,0,0, 1)
			);
			project.z = 0;
			break;
		case 2:
			mv = (*view)*mat4 (
				vec4(0,0,0, 0),
				vec4(0,1,0, 0),
				vec4(0,0,1, 0),
				vec4(0,0,0, 1)
			);
			project.x = 0;
			break;
		case 3:
			mv = (*view)*mat4 (
				vec4(1,0,0, 0),
				vec4(0,0,0, 0),
				vec4(0,0,1, 0),
				vec4(0,0,0, 1)
			);
			project.y = 0;
			break;
		default:
			mv = (*view)*mat4 (
				vec4(1,0,0, position.x),
				vec4(0,1,0, position.y),
				vec4(0,0,1, position.z),
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


void CPolygonalPrismObject::rotate(vec3 rot)
{
	rotation += rot;
	if (rot != vec3(0))
	{
		for (short i = 0; i < vertices.size(); i++)
		{
			vertices[i] = glm::rotate<float, highp>(vertices[i], rot.x, vec3(1, 0, 0));
			vertices[i] = glm::rotate<float, highp>(vertices[i], rot.y, vec3(0, 1, 0));
			vertices[i] = glm::rotate<float, highp>(vertices[i], rot.z, vec3(0, 0, -1));
			normals[i] = glm::rotate<float, highp>(normals[i], rot.x, vec3(1, 0, 0));
			normals[i] = glm::rotate<float, highp>(normals[i], rot.y, vec3(0, 1, 0));
			normals[i] = glm::rotate<float, highp>(normals[i], rot.z, vec3(0, 0, -1));
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec3), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(vec3), &normals[0], GL_STATIC_DRAW);
}

GLboolean CPolygonalPrismObject::selectionRayTry(vec3, vec3, vec3 &)
{
	return GLboolean();
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
		normals[3 * i] = normalize(findNormal(vertices[3 * i], vertices[3 * i + 1], vertices[3 * i + 2]));
		normals[3 * i + 1] = normals[3 * i];
		normals[3 * i + 2] = normals[3 * i];

	}
	for (short i = 0; i <  vertices.size(); i++)
	{
		vertices[i] = glm::rotate<float, highp>(vertices[i], (float)M_PI / sides, vec3(0, 1, 0));
		normals[i] = glm::rotate<float, highp>(normals[i], (float)M_PI / sides, vec3(0, 1, 0));
	}

	if (rot != vec3(0))
	{
		for (short i = 0; i < vertices.size(); i++)
		{
			vertices[i] = glm::rotate<float, highp>(vertices[i], rot.x, vec3(1, 0, 0));
			vertices[i] = glm::rotate<float, highp>(vertices[i], rot.y, vec3(0, 1, 0));
			vertices[i] = glm::rotate<float, highp>(vertices[i], rot.z, vec3(0, 0, -1));
			normals[i] = glm::rotate<float, highp>(normals[i], rot.x, vec3(1, 0, 0));
			normals[i] = glm::rotate<float, highp>(normals[i], rot.y, vec3(0, 1, 0));
			normals[i] = glm::rotate<float, highp>(normals[i], rot.z, vec3(0, 0, -1));
		}
	}
	glBindVertexArray(vao);
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec3), &vertices[0], GL_STATIC_DRAW);
	glGenBuffers(1, &normalsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(vec3), &normals[0], GL_STATIC_DRAW);

}

CPolygonalPrismObject::~CPolygonalPrismObject()
{
	vertices.clear();
	indices.clear();
	normals.clear();
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &elementBuffer);
	glDeleteBuffers(1, &normalsBuffer);

}

