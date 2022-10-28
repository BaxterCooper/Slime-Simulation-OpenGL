#include <Shader.hpp>

std::string get_file_contents(const char* filepath) {
	std::ifstream in(filepath, std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();

		return contents;
	}

	throw(errno);
}

Shader::Shader(const char* filepath, GLenum shaderType) {
	std::string shaderCode = get_file_contents(filepath);

	const char* shaderSource = shaderCode.c_str();

	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	ID = shader;
}