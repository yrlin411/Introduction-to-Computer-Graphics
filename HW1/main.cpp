#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Object.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
unsigned int createShader(const char *filename, const char *type);
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader);
unsigned int ModelVAO(Object *model);
void DrawModel(const char *target, glm::mat4 M, glm::mat4 V, glm::mat4 P);

Object *catModel = new Object("obj/cat.obj");
Object *baseModel = new Object("obj/platform.obj");
Object *teapotModel = new Object("obj/teapot.obj");
Object *ballModel = new Object("obj/tennis_ball.obj");
Object *diskModel = new Object("obj/disk.obj");
unsigned int catVAO, baseVAO, ballVAO, teapotVAO, diskVAO;
unsigned int shaderProgram;
int windowWidth = 800, windowHeight = 600;
float speed = 1.0;

int main()
{
	// Initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "HW1", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// VAO, VBO
	catVAO = ModelVAO(catModel);
	baseVAO = ModelVAO(baseModel);
	ballVAO = ModelVAO(ballModel);
	teapotVAO = ModelVAO(teapotModel);
	diskVAO = ModelVAO(diskModel);

	// Shaders
	unsigned int vertexShader, fragmentShader;
	vertexShader = createShader("vertexShader.vert", "vert");
	fragmentShader = createShader("fragmentShader.frag", "frag");
	shaderProgram = createProgram(vertexShader, fragmentShader);
	glUseProgram(shaderProgram);

	// TO DO:
	// Enable depth test, face culling
	// depth test on each fragment
	glEnable(GL_DEPTH_TEST); // if passed the fragment is rendered, if not, discarded
	glDepthFunc(GL_LEQUAL);	 // test passed if fragment depth <= depth stored in buffer

	// face culling: discard the face not visible to reduce the number of faces
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW); // define face with specified ordered vertices as front (CCW: counter-clockwise)
	glCullFace(GL_BACK); // cull

	// Display loop
	// TO DO:
	// Draw the rotating music box
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);

	// view matrix
	glm::mat4 view;
	view = glm::lookAt(glm::vec3(0, 12, 12), glm::vec3(0, -1, 0), glm::vec3(0, 1, 0));

	// perspective matrix
	glm::mat4 perspective;
	perspective = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

	float rotateBase = -0.2, rotateTisk = -1.0, rotateDisk = -0.5, rotateBall = 1.2;

	while (!glfwWindowShouldClose(window))
	{
		// TO DO:
		// Create model, view, and perspective matrix

		// clear the buffers
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // reset the color buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Base -> TeapotDisk -> Teapot
		// Base -> Disk -> Cat -> Ball

		glm::mat4 modelBase = glm::mat4(1.0f);
		modelBase = glm::rotate(modelBase, glm::radians(rotateBase), glm::vec3(0, 1, 0));

		glm::mat4 modelTisk = glm::mat4(1.0f);
		modelTisk = glm::translate(modelBase, glm::vec3(-1, 1.5, 0));
		modelTisk = glm::rotate(modelTisk, glm::radians(rotateTisk), glm::vec3(0, 1, 0));

		glm::mat4 modelDisk = glm::mat4(1.0f);
		modelDisk = glm::translate(modelBase, glm::vec3(0, 1.3, 0));
		modelDisk = glm::rotate(modelDisk, glm::radians(rotateDisk), glm::vec3(0, 1, 0));

		glm::mat4 modelPot = glm::mat4(1.0f);
		modelPot = glm::translate(modelTisk, glm::vec3(0, 0, 0));

		glm::mat4 modelCat = glm::mat4(1.0f);
		modelCat = glm::translate(modelDisk, glm::vec3(2.5, 0, 0));

		glm::mat4 modelBall = glm::mat4(1.0f);
		modelBall = glm::translate(modelCat, glm::vec3(0, 0.5, 2));
		modelBall = glm::rotate(modelBall, glm::radians(rotateBall), glm::vec3(1, 0, 0));

		modelBase = glm::scale(modelBase, glm::vec3(1.25, 1, 1.25));
		modelDisk = glm::scale(modelDisk, glm::vec3(4, 1, 4));
		modelPot = glm::scale(modelPot, glm::vec3(0.3, 0.3, 0.3));
		modelBall = glm::scale(modelBall, glm::vec3(1.2, 1.2, 1.2));

		DrawModel("base", modelBase, view, perspective);
		DrawModel("disk", modelTisk, view, perspective);
		DrawModel("disk", modelDisk, view, perspective);
		DrawModel("teapot", modelPot, view, perspective);
		DrawModel("cat", modelCat, view, perspective);
		DrawModel("ball", modelBall, view, perspective);

		rotateBase = rotateBase - 0.2;
		rotateTisk = rotateTisk - 1;
		rotateDisk = rotateDisk - (0.5 * speed);
		rotateBall = rotateBall + 1.2;
		if (rotateBase < -360)
			rotateBase = rotateBase + 360;
		if (rotateTisk < -360)
			rotateTisk = rotateTisk + 360;
		if (rotateDisk < -360)
			rotateDisk = rotateDisk + 360;
		if (rotateBall > 360)
			rotateBall = rotateBall + 360;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// TO DO:
// Add key callback
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		speed = speed * 2;
	}
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		speed = speed / 2;
	}
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}

unsigned int createShader(const char *filename, const char *type)
{
	unsigned int shader;
	if (strcmp(type, "vert") == 0)
		shader = glCreateShader(GL_VERTEX_SHADER);
	else if (strcmp(type, "frag") == 0)
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	else
	{
		cout << "Unknown Shader Type.\n";
		return 0;
	}

	FILE *fp = fopen(filename, "rb");
	fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET); // same as rewind(fp);

	char *source = (char *)malloc(sizeof(char) * (fsize + 1));
	fread(source, fsize, 1, fp);
	fclose(fp);

	source[fsize] = 0;

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::" << type << "::COMPLIATION_FAILED\n"
			 << infoLog << endl;
		return -1;
	}

	return shader;
}

unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
	unsigned int program = glCreateProgram();

	// Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	// Link our program
	glLinkProgram(program);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	int success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		int maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		char *infoLog = (char *)malloc(sizeof(char) * (maxLength));
		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);

		// We don't need the program anymore.
		glDeleteProgram(program);
		// Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		puts(infoLog);
		free(infoLog);

		return -1;
	}

	// Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	return program;
}

unsigned int ModelVAO(Object *model)
{
	unsigned int VAO, VBO[3];
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(3, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model->positions.size()), &(model->positions[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model->normals.size()), &(model->normals[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model->texcoords.size()), &(model->texcoords[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}

void DrawModel(const char *target, glm::mat4 M, glm::mat4 V, glm::mat4 P)
{
	unsigned int mLoc, vLoc, pLoc;
	mLoc = glGetUniformLocation(shaderProgram, "M");
	vLoc = glGetUniformLocation(shaderProgram, "V");
	pLoc = glGetUniformLocation(shaderProgram, "P");
	glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(M));
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(V));
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(P));
	if (strcmp(target, "base") == 0)
	{
		glBindVertexArray(baseVAO);
		glDrawArrays(GL_TRIANGLES, 0, baseModel->positions.size());
	}
	else if (strcmp(target, "cat") == 0)
	{
		glBindVertexArray(catVAO);
		glDrawArrays(GL_TRIANGLES, 0, catModel->positions.size());
	}
	else if (strcmp(target, "ball") == 0)
	{
		glBindVertexArray(ballVAO);
		glDrawArrays(GL_TRIANGLES, 0, ballModel->positions.size());
	}
	else if (strcmp(target, "teapot") == 0)
	{
		glBindVertexArray(teapotVAO);
		glDrawArrays(GL_TRIANGLES, 0, teapotModel->positions.size());
	}
	else if (strcmp(target, "disk") == 0)
	{
		glBindVertexArray(diskVAO);
		glDrawArrays(GL_TRIANGLES, 0, diskModel->positions.size());
	}
	glBindVertexArray(0);
}
