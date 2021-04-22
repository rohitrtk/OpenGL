#pragma once

// OpenGL Debugging

#define ASSERT(x) if(!(x)) __debugbreak()
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cerr << "OpenGL Error (" << function << ", "
			<< file << ", " << line << "): " << error << std::endl;
		return false;
	}
	return true;
}

// Other

#define LOG(x) std::cout << x << std::endl
