#include "Shader.h"

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <exception>

#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath) 
{
	std::string vertexCode, fragmentCode;
	std::ifstream vShaderFile, fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	
	try 
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& ex) 
	{ 
		std::cerr << ex.what() << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	GLuint vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	this->checkShaderCompilation(vertex);

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	this->checkShaderCompilation(fragment);

	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertex);
	glAttachShader(this->ID, fragment);
	glLinkProgram(this->ID);
	this->checkShaderLinking(this->ID);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() 
{
	glUseProgram(this->ID);
}

void Shader::checkShaderCompilation(const unsigned int& shader) 
{
	int success;
	char infoLog[INFO_LOG_SIZE];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		glGetShaderInfoLog(shader, INFO_LOG_SIZE, NULL, infoLog);
		std::cerr << "Error, compilation of shader failed!\n" << infoLog << std::endl;
	}
}

void Shader::checkShaderLinking(const unsigned int& shader)
{

	int success;
	char infoLog[INFO_LOG_SIZE];

	glGetShaderiv(shader, GL_LINK_STATUS, &success);
	if (!success) 
	{
		glGetShaderInfoLog(shader, INFO_LOG_SIZE, NULL, infoLog);
		std::cerr << "Error, linking of shader failed!\n" << infoLog << std::endl;
	}
}

void Shader::setBool(const std::string& name, bool value) const 
{
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string& name, int value) const 
{
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const 
{
	glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setVec3f(const std::string& name, float x, float y, float z) const 
{
	glUniform3f(glGetUniformLocation(this->ID, name.c_str()), x, y, z);
}

void Shader::setMat4fv(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

