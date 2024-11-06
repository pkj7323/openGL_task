#pragma once
#include "pch.h"
class Shader
{
	SINGLE(Shader)
public:
	void make_ShaderProgram(const string& vertexPath, const string& fragmentPath);
	void Use()
	{
		glUseProgram(shaderProgramID);
	}
	GLuint GetID() {
		return this->shaderProgramID;
	}
private:
	GLint	make_fragmentShaders(const string& fragmentfile);
	GLint	make_vertexShaders(const string& vertexfile);
	string	filetobuf(const string& filename);
	GLuint	shaderProgramID; //--- 세이더 프로그램이름
};

