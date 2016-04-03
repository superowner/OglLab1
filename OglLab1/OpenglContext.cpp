#include "stdafx.h"
#include "OpenglContext.h"
#include <fstream>
#define SHADERV_ "res/shader.vert"
#define SHADERF_ "res/shader.frag"
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}




void COpenglContext::moveSelected()
{
}

COpenglContext::COpenglContext()
{
	ambientCoef = 0.2;
	lightColor = vec3(1, 1, 1);
	lAmbient = ambientCoef*lightColor;
	lPos = vec3(10, -10, -10);
	lShine = 0.5f;
	lStrength = 0.5f;
	width = 800; height = 600;
	editor = new Workspace();
	Init();

}


COpenglContext::~COpenglContext()
{
}

void COpenglContext::Init()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	//shaders loading block
	prog = LoadShaders(SHADERV_, SHADERF_);

	ColourID = glGetUniformLocation(prog, "Color");

	
	MatrixID_MV = glGetUniformLocation(prog, "MV");
	MatrixID_Projection = glGetUniformLocation(prog, "P");
	LightVecAmbientID = glGetUniformLocation(prog, "LightAmbient");
	LightPosID = glGetUniformLocation(prog, "LightPos");
	LightVecColorID = glGetUniformLocation(prog, "LightColor");

	LightShineID = glGetUniformLocation(prog, "LightShine");
	LightStrengthID = glGetUniformLocation(prog, "LightStrength");

	enableDirectLightID = glGetUniformLocation(prog, "enableDirectLight");

	translateID = glGetUniformLocation(prog, "translate");


	



	glUseProgram( prog);
	// Compute the MVP matrix
	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform

	glUniformMatrix4fv( MatrixID_Projection, 1, GL_FALSE, &editor->proj[0][0]);
	glUniform3fv( LightVecAmbientID, 1, & lAmbient[0]);
	glUniform3fv( LightPosID, 1, & lPos[0]);
	glUniform3fv( LightVecColorID, 1, & lightColor[0]);

	glUniform1f( LightShineID,  lShine);
	glUniform1f( LightStrengthID,  lStrength);
	
	
}
void COpenglContext::Draw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(this->prog);

	for (int i = 0; i < fieldObjects.size();i++)
	{
		fieldObjects[i]->draw(vao, translateID, ColourID, MatrixID_MV, &editor->view, enableDirectLightID);
	}
}

void COpenglContext::resize(int x, int y)
{
	if (x != 0 && y != 0)
	{
		height = y; width = x;
		editor->recProj(x,y);
		glUniformMatrix4fv(MatrixID_Projection, 1, GL_FALSE, &editor->proj[0][0]);
	}
}

void COpenglContext::selectObject(bool multiple, POINT &clickPoint)
{
	vec3 ray_wor = editor->rayCalc(clickPoint);
	
	vec3 intersect=vec3(10000, 10000, 10000); short selected=-1;
	for (int i = 0; i < fieldObjects.size(); i++)
	{
		vec3 temp_intersect;
		if (fieldObjects[i]->selectionRayTry(ray_wor, editor->pov, temp_intersect))
		{
			if (distance(editor->pov, temp_intersect) < distance(editor->pov, intersect))
			{
				intersect = temp_intersect;
				selected = i;
			}
		}
	}
	if (selected >= 0)
	{
		if (multiple)
		{
			if (selectedObjects.count(selected))
			{
				selectedObjects.erase(selected);
				fieldObjects[selected]->selectionMode = 0;
			}
			else
			{
				selectedObjects.insert(selected);
				fieldObjects[selected]->selectionMode = 1;
			}
		}
		else
		{
				for (auto j = selectedObjects.begin(); j != selectedObjects.end(); j++)
					fieldObjects[*j]->selectionMode = 0;
				selectedObjects.clear();
				selectedObjects.insert(selected);
				fieldObjects[selected]->selectionMode = 1;
				editor->setSelected(fieldObjects[selected]);
			}
	}
	else
	{
		for (auto j = selectedObjects.begin(); j != selectedObjects.end(); j++)
				fieldObjects[(*j)]->selectionMode = 0;
			selectedObjects.clear();
			editor->remSelected();
	}

	return;
}

void COpenglContext::addObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, GLuint sidesNum, GLfloat height, GLfloat width, glm::vec4 color)
{

	CPolygonalPrismObject* a = new CPolygonalPrismObject(vao, pos, rot, scale, sidesNum, height, width, color);
	fieldObjects.push_back(a);
}
