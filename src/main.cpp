#include <iostream>
#include <cmath>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <Shader.hpp>
#include <Agent.hpp>

#include <config.hpp>


const unsigned short OPENGL_MAJOR_VERSION = 4;
const unsigned short OPENGL_MINOR_VERSION = 6;


Agent *createAgents() {
	Agent *agents = new Agent[AGENT_COUNT];
	for (int i = 0; i < AGENT_COUNT; i++) {
		agents[i] = Agent();
	}
	return agents;
}

GLfloat *getAgentPositions(Agent *agents) {
	GLfloat *positions = new GLfloat[AGENT_COUNT * 2];
	for (int i = 0; i < AGENT_COUNT; i++) {
		positions[i*2] = agents[i].position.x;
		positions[i*2 + 1] = agents[i].position.y;
	}
	return positions;
}

GLfloat *getAgentDirections(Agent *agents) {
	GLfloat *directions = new GLfloat[AGENT_COUNT * 2];
	for (int i = 0; i < AGENT_COUNT; i++) {
		directions[i*2] = agents[i].direction.x;
		directions[i*2 + 1] = agents[i].direction.y;
	}
	return directions;
}


int main() {
	srand(time(NULL));
	// ------------------------------------------------------------------------
	// WINDOW INITALISATION

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
	glfwSwapInterval(true);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

		std::cout << "Failed to initialize OpenGL context\n";
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	

	// ------------------------------------------------------------------------
	// SHADERS

	Shader agentVertexShader = Shader("./shaders/agent.vert", GL_VERTEX_SHADER);
	Shader agentFragmentShader = Shader("./shaders/agent.frag", GL_FRAGMENT_SHADER);

	Shader screenVertexShader = Shader("./shaders/screen.vert", GL_VERTEX_SHADER);
	Shader screenFragmentShader = Shader("./shaders/screen.frag", GL_FRAGMENT_SHADER);

	Shader processVertexShader = Shader("./shaders/process.vert", GL_VERTEX_SHADER);
	Shader processFragmentShader = Shader("./shaders/process.frag", GL_FRAGMENT_SHADER);

	Shader agentComputeShader = Shader("./shaders/agent.comp", GL_COMPUTE_SHADER);


	// ------------------------------------------------------------------------
	// PROGRAMS

	GLuint agentShaderProgram = glCreateProgram();
	glAttachShader(agentShaderProgram, agentVertexShader.ID);
	glAttachShader(agentShaderProgram, agentFragmentShader.ID);
	glLinkProgram(agentShaderProgram);

	glDeleteShader(agentVertexShader.ID);
	glDeleteShader(agentFragmentShader.ID);


	GLuint screenShaderProgram = glCreateProgram();
	glAttachShader(screenShaderProgram, screenVertexShader.ID);
	glAttachShader(screenShaderProgram, screenFragmentShader.ID);
	glLinkProgram(screenShaderProgram);

	glDeleteShader(screenVertexShader.ID);
	glDeleteShader(screenFragmentShader.ID);


	GLuint processShaderProgram = glCreateProgram();
	glAttachShader(processShaderProgram, processVertexShader.ID);
	glAttachShader(processShaderProgram, processFragmentShader.ID);
	glLinkProgram(processShaderProgram);

	glDeleteShader(processVertexShader.ID);
	glDeleteShader(processFragmentShader.ID);


	GLuint agentComputeProgram = glCreateProgram();
	glAttachShader(agentComputeProgram, agentComputeShader.ID);
	glLinkProgram(agentComputeProgram);

	glDeleteShader(agentComputeShader.ID);


	// ------------------------------------------------------------------------
	// TEXTURES

	GLuint screenTexture;
	glCreateTextures(GL_TEXTURE_2D, 1, &screenTexture);
	glTextureParameteri(screenTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(screenTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(screenTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(screenTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureStorage2D(screenTexture, 1, GL_RGBA32F, WINDOW_WIDTH, WINDOW_HEIGHT);
	glBindImageTexture(0, screenTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	

	// ------------------------------------------------------------------------
	// DATA
	
	Agent *agents = createAgents();

	GLfloat *agentPositions = getAgentPositions(agents);
	GLfloat *agentDirections = getAgentDirections(agents);


	GLfloat screenVertices[] = {
		-1.0, -1.0,
		-1.0,  1.0,
		 1.0,  1.0,
		 1.0, -1.0,
	};

	GLuint screenIndices[] = {
		0, 2, 1,
		0, 2, 3
	};
	
	// ------------------------------------------------------------------------
	// OBJECTS

	GLuint agentVAO1, agentVBO1, agentVAO2, agentVBO2;
	glCreateVertexArrays(1, &agentVAO1);
	glCreateBuffers(1, &agentVBO1);
	glCreateVertexArrays(1, &agentVAO2);
	glCreateBuffers(1, &agentVBO2);

	glNamedBufferData(agentVBO1, sizeof(GLfloat) * 2 * AGENT_COUNT, agentPositions, GL_STATIC_DRAW);
	glNamedBufferData(agentVBO2, sizeof(GLfloat) * 2 * AGENT_COUNT, agentDirections, GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(agentVAO1, 0);
	glVertexArrayAttribBinding(agentVAO1, 0, 0);
	glVertexArrayAttribFormat(agentVAO1, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexArrayAttrib(agentVAO2, 0);
	glVertexArrayAttribBinding(agentVAO2, 0, 0);
	glVertexArrayAttribFormat(agentVAO2, 0, 2, GL_FLOAT, GL_FALSE, 0);

	glVertexArrayVertexBuffer(agentVAO1, 0, agentVBO1, 0, 2 * sizeof(GLfloat));
	glVertexArrayVertexBuffer(agentVAO2, 0, agentVBO2, 0, 2 * sizeof(GLfloat));


	GLuint screenVAO, screenVBO, screenEBO;
	glCreateVertexArrays(1, &screenVAO);
	glCreateBuffers(1, &screenVBO);
	glCreateBuffers(1, &screenEBO);

	glNamedBufferData(screenVBO, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);
	glNamedBufferData(screenEBO, sizeof(screenIndices), screenIndices, GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(screenVAO, 0);
	glVertexArrayAttribBinding(screenVAO, 0, 0);
	glVertexArrayAttribFormat(screenVAO, 0, 2, GL_FLOAT, GL_FALSE, 0);

	glVertexArrayVertexBuffer(screenVAO, 0, screenVBO, 0, 2 * sizeof(GLfloat));
	glVertexArrayElementBuffer(screenVAO, screenEBO);


	// ------------------------------------------------------------------------
	// MAIN WHILE LOOP
	
	while (!glfwWindowShouldClose(window)) {
		// BIND SCREEN TEXTURE
		glBindImageTexture(0, screenTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

		// AGENT SHADER
		glUseProgram(agentShaderProgram);
		glUniform1f(glGetUniformLocation(agentShaderProgram, "agentLuminance"), AGENT_LUMINANCE);
		glBindVertexArray(agentVAO1);
		glDrawArrays(GL_POINTS, 0, AGENT_COUNT);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		// SCREEN SHADER
		glUseProgram(screenShaderProgram);
		glBindVertexArray(screenVAO);
		glDrawElements(GL_TRIANGLES, sizeof(screenIndices) / sizeof(screenIndices[0]), GL_UNSIGNED_INT, 0);

		// PROCESS SHADER
		glUseProgram(processShaderProgram);
		glUniform1f(glGetUniformLocation(processShaderProgram, "blurSpeed"), BLUR_SPEED);
		glUniform1f(glGetUniformLocation(processShaderProgram, "fadeSpeed"), FADE_SPEED);
		glBindVertexArray(screenVAO);
		glDrawElements(GL_TRIANGLES, sizeof(screenIndices) / sizeof(screenIndices[0]), GL_UNSIGNED_INT, 0);
		glMemoryBarrier(GL_ALL_BARRIER_BITS); // can be optimised

		// AGENT COMPUTE
		glUseProgram(agentComputeProgram);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, agentVAO1);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, agentVAO2);
		glUniform1f(glGetUniformLocation(agentComputeProgram, "agentMoveSpeed"), AGENT_MOVE_SPEED);
		glUniform1f(glGetUniformLocation(agentComputeProgram, "agentTurnSpeed"), AGENT_TURN_SPEED);
		glUniform1i(glGetUniformLocation(agentComputeProgram, "sensorSize"), SENSOR_SIZE);
		glUniform1f(glGetUniformLocation(agentComputeProgram, "sensorOffset"), SENSOR_OFFSET);
		glUniform1f(glGetUniformLocation(agentComputeProgram, "sensorFOV"), SENSOR_FOV);
		glUniform1i(glGetUniformLocation(agentComputeProgram, "time"), time(NULL));
		glDispatchCompute(AGENT_COUNT / 8, AGENT_COUNT / 8, 1); // can be optimised
		glMemoryBarrier(GL_ALL_BARRIER_BITS); // can be optimised

		glfwSwapBuffers(window);
		glfwPollEvents();

		std::cout << glGetError() << "\n";
	}


	// ------------------------------------------------------------------------
	// CLEANUP

	glDeleteVertexArrays(1, &agentVAO1);
	glDeleteBuffers(1, &agentVBO1);

	glDeleteVertexArrays(1, &agentVAO2);
	glDeleteBuffers(1, &agentVBO2);

	glDeleteVertexArrays(1, &screenVAO);
	glDeleteBuffers(1, &screenVBO);
	glDeleteBuffers(1, &screenEBO);

	glDeleteProgram(agentShaderProgram);
	glDeleteProgram(screenShaderProgram);
	glDeleteProgram(processShaderProgram);
	glDeleteProgram(agentComputeProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}