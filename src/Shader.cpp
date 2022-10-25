#include <Shader.hpp>

std::string get_file_contents(const char* filename) {
	std::ifstream in(filename, std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}

	throw(errno);
}

Shader::Shader(const char* filename, int shaderType) {
    ID = glCreateShader(shaderType);

	// Read vertexFile and fragmentFile and store the strings
	std::string shaderCode = get_file_contents(filename);

	// Convert the shader source strings into character arrays
	const char* shaderSource = shaderCode.c_str();

	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(ID, 1, &shaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(ID);
}