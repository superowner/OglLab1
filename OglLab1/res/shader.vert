#version 410 core

uniform mat4 MV;
uniform mat4 P;
uniform vec3 translate;
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

out vec4 mvPos;
out vec3 N;
void main(){
	
	N=normalize(((transpose(inverse(MV)))*vec4(vertexNormal,1)).xyz); //normal
	mvPos = MV*vec4(vertexPosition+translate, 1);
	gl_Position =  P* mvPos;
	
}