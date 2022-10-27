#include <iostream>
#include <cmath>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <Shader.hpp>
#include <Agent.hpp>

// #include <config.hpp>

const unsigned short OPENGL_MAJOR_VERSION = 4;
const unsigned short OPENGL_MINOR_VERSION = 6;

const bool vSync = true;

Agent *createAgents() {
	Agent *agents = new Agent[AGENT_COUNT];
	for (int i = 0; i < AGENT_COUNT; i++) {
		agents[i] = Agent();
	}
	return agents;
}

GLfloat *getAgentVertices(Agent *agents) {
	GLfloat *vertices = new GLfloat[AGENT_COUNT * 2];
	for (int i = 0; i < AGENT_COUNT; i++) {
		vertices[i*2] = agents[i].position.x;
		vertices[i*2 + 1] = agents[i].position.y;
	}
	return vertices;
}

int main() {
	// ------------------------------------------------------------------------
	// WINDOW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Compute Shaders", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create the GLFW window\n";
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(vSync);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

		std::cout << "Failed to initialize OpenGL context\n";
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// ------------------------------------------------------------------------
	// SCREEN TEXTURE

	GLuint screenTex;
	glCreateTextures(GL_TEXTURE_2D, 1, &screenTex);
	glTextureParameteri(screenTex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(screenTex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(screenTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(screenTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureStorage2D(screenTex, 1, GL_RGBA32F, WINDOW_WIDTH, WINDOW_HEIGHT);
	glBindImageTexture(0, screenTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	// ------------------------------------------------------------------------
	// AGENT SHADERS

	Shader agentVertexShader("./shaders/agent.vert", GL_VERTEX_SHADER);
	Shader agentFragmentShader("./shaders/agent.frag", GL_FRAGMENT_SHADER);

	GLuint agentShaderProgram = glCreateProgram();
	glAttachShader(agentShaderProgram, agentVertexShader.ID);
	glAttachShader(agentShaderProgram, agentFragmentShader.ID);
	glLinkProgram(agentShaderProgram);

	glDeleteShader(agentVertexShader.ID);
	glDeleteShader(agentFragmentShader.ID);

	Agent *agents = createAgents();
	glEnable(GL_PROGRAM_POINT_SIZE);
	GLfloat *agentVertices = getAgentVertices(agents);

	GLuint agentVAO, agentVBO;
	glGenVertexArrays(1, &agentVAO);
	glGenBuffers(1, &agentVBO);

	glBindVertexArray(agentVAO);
	glBindBuffer(GL_ARRAY_BUFFER, agentVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * AGENT_COUNT, agentVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// ------------------------------------------------------------------------
	// SCREEN SHADER

	Shader screenVertexShader("./shaders/screen.vert", GL_VERTEX_SHADER);
	Shader screenFragmentShader("./shaders/screen.frag", GL_FRAGMENT_SHADER);

	GLuint screenShaderProgram = glCreateProgram();
	glAttachShader(screenShaderProgram, screenVertexShader.ID);
	glAttachShader(screenShaderProgram, screenFragmentShader.ID);
	glLinkProgram(screenShaderProgram);

	glDeleteShader(screenVertexShader.ID);
	glDeleteShader(screenFragmentShader.ID);

	GLfloat screenVertices[] =
	{
		-1.0f, -1.0f,
		-1.0f,  1.0f,
		1.0f,  1.0f,
		1.0f, -1.0f
	};

	GLuint screenIndices[] =
	{
		0, 2, 1,
		0, 3, 2
	};

	GLuint screenVAO, screenVBO, screenEBO;
	glCreateVertexArrays(1, &screenVAO);
	glCreateBuffers(1, &screenVBO);
	glCreateBuffers(1, &screenEBO);

	glNamedBufferData(screenVBO, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);
	glNamedBufferData(screenEBO, sizeof(screenIndices), screenIndices, GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(screenVAO, 0);
	glVertexArrayAttribBinding(screenVAO, 0, 0);
	glVertexArrayAttribFormat(screenVAO, 0, 2, GL_FLOAT, GL_FALSE, 0);

	glEnableVertexArrayAttrib(screenVAO, 1);
	glVertexArrayAttribBinding(screenVAO, 1, 0);
	glVertexArrayAttribFormat(screenVAO, 1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat));

	glVertexArrayVertexBuffer(screenVAO, 0, screenVBO, 0, 2 * sizeof(GLfloat));
	glVertexArrayElementBuffer(screenVAO, screenEBO);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// AGENT SHADER
		glUseProgram(agentShaderProgram);
		glBindTextureUnit(0, screenTex);
		glUniform1i(glGetUniformLocation(agentShaderProgram, "screen"), 0);
		glBindVertexArray(agentVAO);
		glDrawArrays(GL_POINTS, 0, AGENT_COUNT);

		// SCREEN SHADER
		// glUseProgram(screenShaderProgram);
		// glBindTextureUnit(0, screenTex);
		// glUniform1i(glGetUniformLocation(screenShaderProgram, "screen"), 0);
		// glBindVertexArray(screenVAO);
		// glDrawElements(GL_TRIANGLES, sizeof(screenIndices) / sizeof(screenIndices[0]), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	// Delete all the objects we've created
	glDeleteVertexArrays(1, &agentVAO);
	glDeleteBuffers(1, &agentVBO);
	glDeleteProgram(agentShaderProgram);
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}