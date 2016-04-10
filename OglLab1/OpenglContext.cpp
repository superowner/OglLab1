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

void COpenglContext::setCursor(CPoint a)
{
	cursor = a;
}

void COpenglContext::mouseWheel(short k)
{
	editor->changeDistance(k / 200.0);
}

void COpenglContext::setMode(mode _mode)
{
	Mode = _mode;
}

mode COpenglContext::getMode()
{
	return Mode;
}

void COpenglContext::LButtonMove(CPoint pos)
{
	POINT shift;
	shift.x = pos.x - cursor.x;
	shift.y = pos.y - cursor.y;

	//lbutton
	//click - select
	//case selection:
	if (Mode == selection) {
		editor->shiftCamera(shift);
		return;
	}
	if (selectedObjects.size())
	{
		switch (Mode)
		{

		case movement:
			editor->shiftSelected(shift);
			break;
		case rotation:
			editor->rotate(shift);
			break;

		case colour:
			editor->shiftColor(shift);
			break;

		default:
			break;
		}
	}
}

void COpenglContext::onSave(HWND hwnd)
{
	onClear(hwnd);
	if (fieldObjects.size() == 0) return;
	
	string strTmp;
		for each(CSceneObject* i in fieldObjects)
		{
			string sql = "INSERT INTO model VALUES ('"
				+ to_string(i->getPos().x) + "', '"
				+ to_string(i->getPos().y) + "', '"
				+ to_string(i->getPos().z) + "', '"
				+ to_string(i->getRotation().x) + "', '"
				+ to_string(i->getRotation().y) + "', '"
				+ to_string(i->getRotation().z) + "', '" 
				+ to_string(i->getScale().x) + "', '"
				+ to_string(i->getScale().y) + "', '"
				+ to_string(i->getScale().z) + "', '"
				+ to_string(i->getNum()) + "', '"
				+ to_string(i->getHeight()) + "', '"
				+ to_string(i->getWidth()) + "', '"
				+ to_string(i->getColor().r) + "', '"
				+ to_string(i->getColor().g) + "', '"
				+ to_string(i->getColor().b) + "', '"
				+ to_string(i->getColor().a) + "', '"
				+ to_string(i->getRotM(0,0)) + "', '"
				+ to_string(i->getRotM(0, 1)) + "', '"
				+ to_string(i->getRotM(0, 2)) + "', '"
				+ to_string(i->getRotM(0, 3)) + "', '"
				+ to_string(i->getRotM(1,0)) + "', '"
				+ to_string(i->getRotM(1, 1)) + "', '"
				+ to_string(i->getRotM(1, 2)) + "', '"
				+ to_string(i->getRotM(1, 3)) + "', '"
				+ to_string(i->getRotM(2,0)) + "', '"
				+ to_string(i->getRotM(2, 1)) + "', '"
				+ to_string(i->getRotM(2, 2)) + "', '"
				+ to_string(i->getRotM(2, 3)) + "', '"
				+ to_string(i->getRotM(3, 0)) + "', '"
				+ to_string(i->getRotM(3, 1)) + "', '"
				+ to_string(i->getRotM(3, 2)) + "', '"
				+ to_string(i->getRotM(3, 3)) + "');";
			//sql += strTmp + ("') AS tmp WHERE NOT EXISTS (SELECT * FROM models WHERE `path` = '") + strTmp + ("') LIMIT 1;");
	
			mysql(&sql[0]);
		}
	
	/*	if (fieldObjects.size() > 0)
			MessageBox(hwnd, L"Data saved!", L"Success", MB_OK | MB_ICONINFORMATION);
		else
			MessageBox(hwnd, L"No data to save!", L"Error", MB_OK | MB_ICONSTOP);*/

}

void COpenglContext::onLoad(HWND hwnd)
{
	//onClear(hwnd);
	vector<vector<string>> records = mysql("SELECT * FROM model;", true,  32);
	
	for each (CSceneObject* var in fieldObjects)
	{
		delete var;
	}
	fieldObjects.clear();
	string buff;
	std::string::size_type sz;     // alias of size_t   // alias of size_t
//poligon.resize(records.size());
	float temp[32];
	for (int j = 0; j < records.size(); j++)
	{
		for (int i = 0; i < 32; i++) {
			temp[i] = stof(records[j][i], 0);
		}
			addObject(vec3(temp[0], temp[1], temp[2]), vec3(temp[3], temp[4], temp[5]), vec3(temp[6], temp[7], temp[8]),
						(int)temp[9], temp[10], temp[11], vec4(temp[12], temp[13], temp[14], temp[15]), 
						{ 
							temp[16], temp[17], temp[18], temp[19], 
							temp[20], temp[21], temp[22], temp[23], 
							temp[24], temp[25], temp[26], temp[27], 
							temp[28], temp[29], temp[30], temp[31]
						});
	}
	/*fieldObjects.clear();
	
		unsigned i = 0;
		while (tmp[i].GetLength() > 0)
		{
			lb_var.AddString(tmp[i]);
			i++;
		}
		MessageBox(hwnd, L"Data loaded!", L"Success", MB_OK | MB_ICONINFORMATION);*/
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
	Mode = selection;
	Init();

}


COpenglContext::~COpenglContext()
{
}

void COpenglContext::Init()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glClearColor(0.8, 0.8, 0.8, 1.0);

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
	
	for (int i = -10; i < 11; i++)
	{
		coordLines.push_back(vec3(-10, 0, i));
		coordLines.push_back(vec3(10, 0, i));
		coordLines.push_back(vec3(i, 0, -10));
		coordLines.push_back(vec3(i, 0, 10));
	}

	glBindVertexArray(vao);
	glGenBuffers(1, &coordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, coordBuffer);
	glBufferData(GL_ARRAY_BUFFER, coordLines.size()*sizeof(vec3), &coordLines[0], GL_STATIC_DRAW);

}
void COpenglContext::Draw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(this->prog);
	
	glLineWidth(2.0);
	mat4 mv = (editor->view);
	bool enable = 0;
	vec4 lineCol = vec4(0, 0, 0, 1);
	glUniform1i(enableDirectLightID, enable);
	glUniform3fv(translateID, 1, &lineCol[0]);
	glUniformMatrix4fv(MatrixID_MV, 1, GL_FALSE, &mv[0][0]);
	
	glUniform4fv(ColourID, 1, &lineCol[0]);

	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, coordBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, coordBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glDrawArrays(GL_LINES, 0, coordLines.size());

	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

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
void COpenglContext::addObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, GLuint sidesNum, GLfloat height, GLfloat width, glm::vec4 color, glm::mat4 rotation)
{
	CPolygonalPrismObject* a = new CPolygonalPrismObject(vao, pos, rot, scale, sidesNum, height, width, color, rotation);
	fieldObjects.push_back(a);
}
void COpenglContext::RButtonMove(CPoint pos)
{
	POINT shift;
	shift.x = pos.x - cursor.x;
	shift.y = pos.y - cursor.y;
	
	//lbutton
	//click - select
	//
	switch (Mode)
	{
	case selection:
		editor->rotateCamera(shift);
		break;
	case movement:
		editor->shiftCamera(shift);
		break;
	case rotation:
		editor->rotateCamera(shift);
		break;
	case colour:
		editor->shiftColor(shift);
		break;
	default:
		break;
	}


}
void COpenglContext::onClear(HWND hwnd)
{

	mysql("DELETE FROM model;");
	//cout << "Data cleared!";
//	MessageBox(hwnd, L"Data cleared!", L"Success", MB_OK | MB_ICONINFORMATION);
}
