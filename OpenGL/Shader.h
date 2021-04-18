#pragma once

#include <glad/glad.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#define INFO_LOG_SIZE 512

class Shader
{
public:
	// Program ID
	GLuint ID;

	Shader(const char* vertexPath, const char* fragmentPath);
	
	// Use the shader
	void use();

	// Utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	void checkShaderCompilation(const GLuint& shader);
	void checkShaderLinking(const GLuint& shader);
};

