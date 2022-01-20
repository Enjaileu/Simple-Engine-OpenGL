#pragma once 
#include <glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <string>
#include <fstream>

#include "Vector2.h"

class Shader
{
public:
	// State
	GLuint id;

	// Constructor
	Shader() : id(0), vs(0), fs(0), gs(0), tcs(0), tes(0) { }

	// Unload the shader
	void Unload();

	// Sets the current shader as active
	Shader& Use();

	// Compiles the shader from given source code
	void Compile(
		const GLchar* vertexSource,
		const GLchar* fragmentSource,
		const GLchar* tessControlSource = nullptr,
		const GLchar* tessEvalSource = nullptr,
		const GLchar* geometrySource = nullptr
	);

	// Utility functions
	void SetFloat(const GLchar* name, GLfloat value);
	void SetInteger(const GLchar* name, GLint value);
	void SetVector2f(const GLchar* name, GLfloat x, GLfloat y);
	void SetVector2f(const GLchar* name, const Vector2& value);


private:
	GLuint vs;
	GLuint fs;
	GLuint tcs;
	GLuint tes;
	GLuint gs;

	void CompileVertexShader(const GLchar* vertexSource);
	void CompileFragmentShader(const GLchar* fragmentSource);
	bool CompileTessControlShader(const GLchar* tessControlSource);
	bool CompileTessEvalShader(const GLchar* tessEvalSource);
	bool CompileGeometryShader(const GLchar* geometrySource);
	void CreateShaderProgram(bool tessShadersExist, bool geometryShaderExists);

	void CheckShaderErrors(GLuint shader, std::string shaderType);
	void PrintShaderInfoLog(GLuint shaderIndex);
	void PrintProgrammeInfoLog(GLuint programme);
	const char* GLTypeToString(GLenum type);
	void PrintAllParams(GLuint programme);
	bool IsValid(GLuint programme);
};