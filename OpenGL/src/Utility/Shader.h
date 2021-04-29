#pragma once

#include <glm/glm.hpp>

#include <string>

constexpr int INFO_LOG_SIZE = 512;

class Shader
{
public:
	// Program ID
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);
	
	// Use the shader
	void use();

	// Utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3f(const std::string& name, float x, float y, float z) const;
	void setMat4fv(const std::string& name, const glm::mat4& mat) const;
	
	void checkShaderCompilation(const unsigned int& shader);
	void checkShaderLinking(const unsigned int& shader);
};

