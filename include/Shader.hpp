#ifndef SHADER
#define SHADER

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filepath);

class Shader {
	public:
		GLuint ID;

		Shader(const char* filepath, GLenum shaderType);
};

#endif