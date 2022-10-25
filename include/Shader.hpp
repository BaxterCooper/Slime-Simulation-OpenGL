#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filename);

class Shader {
	public:
		// Reference ID of the Shader Program
		GLuint ID;

		// Constructor that builds the shader
		Shader(const char* filename, int shaderType);
};
#endif