#include <iostream>
#include <cmath>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_SWIZZLE
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

int main() {
	// create agents
	std::vector<Agent> agents;
	for (int i = 0; i < AGENT_COUNT; i++) {
		agents.push_back(Agent());
	}

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
	const int particles = 64;
    std::vector<glm::vec4> positionData(particles);
    std::vector<glm::vec4> velocityData(particles);
    for(int i = 0;i<particles;++i) {
        // initial position
        positionData[i] = glm::gaussRand(glm::vec4(0,0,0,1), glm::vec4(1, 0.2, 1, 0));
        velocityData[i] = glm::vec4(0.8);
    }

	GLuint positions_vbo, velocities_vbo;
	glGenBuffers(1, &positions_vbo);
    glGenBuffers(1, &velocities_vbo);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocities_vbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4)*particles, &velocityData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, positions_vbo);

    // fill with initial data
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*particles, &positionData[0], GL_STATIC_DRAW);

    // set up generic attrib pointers: do I need these?
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (char*)0 + 0*sizeof(GLfloat));

	const GLuint ssbos[] = {positions_vbo, velocities_vbo};
	glBindBuffersBase(GL_SHADER_STORAGE_BUFFER, 1, 2, ssbos);

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

	// create vertex shader
	Shader vertexShader("./shaders/default.vert", GL_VERTEX_SHADER);
	// create fragment shader
	Shader fragmentShader("./shaders/default.frag", GL_FRAGMENT_SHADER);

	// create shader program
	GLuint shaderProgram = glCreateProgram();

	// attach vertex and fragment shader to shader program
	glAttachShader(shaderProgram, vertexShader.ID);
	glAttachShader(shaderProgram, fragmentShader.ID);

	// link shader program 
	glLinkProgram(shaderProgram);

	// delete vertex and fragement shader
	glDeleteShader(vertexShader.ID);
	glDeleteShader(fragmentShader.ID);

	// create compute shader
	Shader computeShader("./shaders/blur.comp", GL_COMPUTE_SHADER);

	// create compute program
	GLuint computeProgram = glCreateProgram();

	// attach compute shader to shader program
	glAttachShader(computeProgram, computeShader.ID);

	// link shader program
	glLinkProgram(computeProgram);

	// delete compute shader
	glDeleteShader(computeShader.ID);

	int work_grp_cnt[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);
	std::cout << "Max work groups per compute shader" << 
		" x:" << work_grp_cnt[0] <<
		" y:" << work_grp_cnt[1] <<
		" z:" << work_grp_cnt[2] << "\n";

	int work_grp_size[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);
	std::cout << "Max work group sizes" <<
		" x:" << work_grp_size[0] <<
		" y:" << work_grp_size[1] <<
		" z:" << work_grp_size[2] << "\n";

	int work_grp_inv;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
	std::cout << "Max invocations count per work group: " << work_grp_inv << "\n";

	while (!glfwWindowShouldClose(window)) {
		glUseProgram(computeProgram);
		glDispatchCompute(ceil(WINDOW_WIDTH / 8), ceil(WINDOW_HEIGHT / 4), 1);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);
		std::cout << glGetError() << "\n";

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