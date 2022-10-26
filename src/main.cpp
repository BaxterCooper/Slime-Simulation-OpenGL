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

GLfloat vertices[] =
{
	-1.0f, -1.0f , 0.0f, 0.0f, 0.0f,
	-1.0f,  1.0f , 0.0f, 0.0f, 1.0f,
	 1.0f,  1.0f , 0.0f, 1.0f, 1.0f,
	 1.0f, -1.0f , 0.0f, 1.0f, 0.0f,
};

GLuint indices[] =
{
	0, 2, 1,
	0, 3, 2
};

Agent *createAgents() {
	Agent *agents = new Agent[AGENT_COUNT];
	for (int i = 0; i < AGENT_COUNT; i++) {
		agents[i] = Agent();
	}
	return agents;
}

int main() {
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

	// ---------------- BEGIN SHADER DATA ----------------

	// We generate a bunch of positions
	Agent *agents = createAgents();
    std::vector<glm::vec4> agentData(AGENT_COUNT);

	// initial position
    for(int i = 0; i < AGENT_COUNT; i++) {
        glm::vec2 position = agents[i].position;
        glm::vec2 direction = agents[i].direction;
		agentData[i] = glm::vec4(position.x, position.y, direction.x, direction.y);
    }

	GLuint agentDataBuffer;
	glCreateBuffers(1, &agentDataBuffer);

    // fill with initial data
    glBindBuffer(GL_ARRAY_BUFFER, agentDataBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*AGENT_COUNT, &agentData[0], GL_STATIC_DRAW);

    // set up generic attrib pointers: do I need these?
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (char*)0 + 0*sizeof(GLfloat));

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, agentDataBuffer);

	// ---------------- END SHADER DATA ----------------

	GLuint VAO, VBO, EBO;
	glCreateVertexArrays(1, &VAO);
	glCreateBuffers(1, &VBO);
	glCreateBuffers(1, &EBO);

	glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glNamedBufferData(EBO, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(VAO, 0);
	glVertexArrayAttribBinding(VAO, 0, 0);
	glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);

	glEnableVertexArrayAttrib(VAO, 1);
	glVertexArrayAttribBinding(VAO, 1, 0);
	glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));

	glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 5 * sizeof(GLfloat));
	glVertexArrayElementBuffer(VAO, EBO);

	GLuint screenTex;
	glCreateTextures(GL_TEXTURE_2D, 1, &screenTex);
	glTextureParameteri(screenTex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(screenTex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(screenTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(screenTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureStorage2D(screenTex, 1, GL_RGBA32F, WINDOW_WIDTH, WINDOW_HEIGHT);
	glBindImageTexture(0, screenTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	Shader vertexShader("./shaders/default.vert", GL_VERTEX_SHADER);
	Shader fragmentShader("./shaders/default.frag", GL_FRAGMENT_SHADER);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader.ID);
	glAttachShader(shaderProgram, fragmentShader.ID);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader.ID);
	glDeleteShader(fragmentShader.ID);

	Shader computeShader("./shaders/blur.comp", GL_COMPUTE_SHADER);

	GLuint computeProgram = glCreateProgram();
	glAttachShader(computeProgram, computeShader.ID);
	glLinkProgram(computeProgram);

	glDeleteShader(computeShader.ID);

	while (!glfwWindowShouldClose(window)) {
		glUseProgram(computeProgram);
		glUniform1f(glGetUniformLocation(computeProgram, "agentSpeed"), AGENT_SPEED);
		glDispatchCompute(ceil(WINDOW_WIDTH / 8), ceil(WINDOW_HEIGHT / 4), 1);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		glUseProgram(shaderProgram);
		glBindTextureUnit(0, screenTex);
		glUniform1i(glGetUniformLocation(shaderProgram, "screen"), 0);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}