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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
unsigned int createShader(const char* filename, const char* type);
unsigned int createProgram(unsigned int vertexShader, unsigned int geometryShader, unsigned int fragmentShader);
unsigned int ModelVAO(Object* model);
void LoadTexture(unsigned int& texture, const char* tFileName);
glm::mat4 getPerspective();
glm::mat4 getView();

Object* catModel = new Object("obj/cat.obj");
Object* boxModel = new Object("obj/CardboardBox1.obj");
Object* duckModel1 = new Object("obj/duckBody.obj");
Object* duckModel2 = new Object("obj/duckOrange.obj");
Object* duckModel3 = new Object("obj/duckEye.obj");
Object* ballModel = new Object("obj/tennis_ball.obj");

int windowWidth = 1000, windowHeight = 800;

float angle = 0;
float lighting = 1;
float squeezeFactor = 0;
bool squeezing = false;
bool lightChanging = false;
bool scaryDuckTurn = false;
bool duckWalk = true;
bool rotation = true;
int cameraMove = 0;
int crazyDuck = 0;
bool showBall = false;
float magic = 0;
float magic2 = 0;

int main()
{
	// Initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFE_OPENGL_FORWARD_COMPACT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "HW4", NULL, NULL);
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

	// Shaders
	unsigned int vertexShader, fragmentShader, geometryShader, shaderProgram;
	vector<unsigned int> catPrograms;
	vertexShader = createShader("shaders/cat.vert", "vert");
	fragmentShader = createShader("shaders/cat.frag", "frag");
	shaderProgram = createProgram(vertexShader, 0, fragmentShader);
	catPrograms.push_back(shaderProgram);

	vector<unsigned int> duckPrograms;
	vertexShader = createShader("shaders/duck.vert", "vert");
	fragmentShader = createShader("shaders/duck.frag", "frag");
	shaderProgram = createProgram(vertexShader, 0, fragmentShader);
	duckPrograms.push_back(shaderProgram);

	vertexShader = createShader("shaders/duckN.vert", "vert");
	geometryShader = createShader("shaders/duckN.geom", "geom");
	fragmentShader = createShader("shaders/duckN.frag", "frag");
	shaderProgram = createProgram(vertexShader, geometryShader, fragmentShader);
	duckPrograms.push_back(shaderProgram);

	vector<unsigned int> ballPrograms;
	vertexShader = createShader("shaders/ball.vert", "vert");
	fragmentShader = createShader("shaders/ball.frag", "frag");
	shaderProgram = createProgram(vertexShader, 0, fragmentShader);
	ballPrograms.push_back(shaderProgram);

	vertexShader = createShader("shaders/ballN.vert", "vert");
	geometryShader = createShader("shaders/ballN.geom", "geom");
	fragmentShader = createShader("shaders/ballN.frag", "frag");
	shaderProgram = createProgram(vertexShader, geometryShader, fragmentShader);
	ballPrograms.push_back(shaderProgram);

	// Texture
	unsigned int catTexture, duck1, duck2, ballTexture;
	LoadTexture(catTexture, "obj/Cat_diffuse.jpg");
	LoadTexture(duck1, "obj/white.png");
	LoadTexture(duck2, "obj/duckOrange.png");
	LoadTexture(ballTexture, "obj/green.jpg");

	// VAO, VBO
	unsigned int catVAO, boxVAO, duck1VAO, duck2VAO, duck3VAO, ballVAO;
	catVAO = ModelVAO(catModel);
	boxVAO = ModelVAO(boxModel);
	duck1VAO = ModelVAO(duckModel1);
	duck2VAO = ModelVAO(duckModel2);
	duck3VAO = ModelVAO(duckModel3);
	ballVAO = ModelVAO(ballModel);

	// Display loop
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	double dt;
	double lastTime = glfwGetTime();
	double currentTime;
	double walk = -10;
	double ballSize = 0;
	unsigned int mLoc, vLoc, pLoc, normalMLoc;

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 perspective = getPerspective();
		glm::mat4 view = getView();

		glm::mat4 model = glm::mat4(1.0f);

		glm::mat4 catM = glm::mat4(1.0f);
		catM = glm::translate(model, glm::vec3(1.5, 0, 0));
		catM = glm::rotate(catM, angle, glm::vec3(0, 1, 0));
		catM = glm::rotate(catM, glm::radians(-90.0f), glm::vec3(0, 1, 0));

		glm::mat4 duckM = glm::mat4(1.0f);
		duckM = glm::translate(model, glm::vec3(-1.5, 0, walk));
		if(scaryDuckTurn) duckM = glm::rotate(duckM, glm::radians(90.0f), glm::vec3(0, 1, 0));

		glm::mat4 ballM = glm::mat4(1.0f);
		ballM = glm::translate(model, glm::vec3(0, 0.1, 0));
		ballM = glm::scale(ballM, glm::vec3(ballSize, ballSize, ballSize));

		glm::mat4 normalM = glm::transpose(glm::inverse(model));

		for (unsigned int program : catPrograms) {
			glUseProgram(program);
			mLoc = glGetUniformLocation(program, "M");
			vLoc = glGetUniformLocation(program, "V");
			pLoc = glGetUniformLocation(program, "P");
			normalMLoc = glGetUniformLocation(program, "normalM");
			glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(catM));
			glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(perspective));
			glUniformMatrix4fv(normalMLoc, 1, GL_FALSE, glm::value_ptr(normalM));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, catTexture);
			glBindVertexArray(catVAO);
			glDrawArrays(GL_TRIANGLES, 0, catModel->positions.size());
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindVertexArray(0);
		}

		for (unsigned int program : duckPrograms) {
			glUseProgram(program);
			mLoc = glGetUniformLocation(program, "M");
			vLoc = glGetUniformLocation(program, "V");
			pLoc = glGetUniformLocation(program, "P");
			normalMLoc = glGetUniformLocation(program, "normalM");
			glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(duckM));
			glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(perspective));
			glUniformMatrix4fv(normalMLoc, 1, GL_FALSE, glm::value_ptr(normalM));
			glUniform1i(glGetUniformLocation(program, "crazy"), crazyDuck);
			glUniform1f(glGetUniformLocation(program, "magic"), magic);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, duck1);
			glBindVertexArray(duck1VAO);
			glDrawArrays(GL_TRIANGLES, 0, duckModel1->positions.size());
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindVertexArray(0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, duck2);
			glBindVertexArray(duck2VAO);
			glDrawArrays(GL_TRIANGLES, 0, duckModel2->positions.size());
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindVertexArray(0);
			glBindVertexArray(duck3VAO);
			glDrawArrays(GL_TRIANGLES, 0, duckModel3->positions.size());
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindVertexArray(0);
		}

		
		for (unsigned int program : ballPrograms) {
			if (magic > 2) {
				glUseProgram(program);
				mLoc = glGetUniformLocation(program, "M");
				vLoc = glGetUniformLocation(program, "V");
				pLoc = glGetUniformLocation(program, "P");
				normalMLoc = glGetUniformLocation(program, "normalM");
				glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(ballM));
				glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(view));
				glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(perspective));
				glUniformMatrix4fv(normalMLoc, 1, GL_FALSE, glm::value_ptr(normalM));
				glUniform1i(glGetUniformLocation(program, "crazy"), crazyDuck);
				glUniform1f(glGetUniformLocation(program, "magic2"), magic2);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, ballTexture);
				glBindVertexArray(ballVAO);
				glDrawArrays(GL_TRIANGLES, 0, ballModel->positions.size());
				glBindTexture(GL_TEXTURE_2D, 0);
				glBindVertexArray(0);
			}
		}

		// Status update
		currentTime = glfwGetTime();
		dt = currentTime - lastTime;
		lastTime = currentTime;
		if (walk < -5) angle += glm::radians(90.0f) * dt;
		if (walk < -0.65) angle += glm::radians(45.0f) * dt;
		if (walk == 0) duckWalk = false;
		if (!duckWalk && crazyDuck) magic += 0.015;
		if (angle > glm::radians(360.0f)) angle -= glm::radians(360.0f);
		if (duckWalk) walk += 0.005;
		if (ballSize < 0.5 && crazyDuck) ballSize += 0.001;
		if (crazyDuck == 2) magic2 += 0.05;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// TO DO:
// Add key callback
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		scaryDuckTurn = !scaryDuckTurn;
		cout << "KEY T PRESSED\n";
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		duckWalk = !duckWalk;
		cout << "KEY W PRESSED\n";
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		crazyDuck++;
		cout << "KEY C PRESSED\n";
	}
	if (key == GLFW_KEY_0 && action == GLFW_PRESS)
	{
		cameraMove = 0;
		cout << "KEY 0 PRESSED\n";
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		cameraMove = 1;
		cout << "KEY 1 PRESSED\n";
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		cameraMove = 2;
		cout << "KEY 2 PRESSED\n";
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		cameraMove = 3;
		cout << "KEY 3 PRESSED\n";
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}

unsigned int createShader(const char* filename, const char* type)
{
	unsigned int shader;
	if (strcmp(type, "vert") == 0)
		shader = glCreateShader(GL_VERTEX_SHADER);
	else if (strcmp(type, "frag") == 0)
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	else if (strcmp(type, "geom") == 0)
		shader = glCreateShader(GL_GEOMETRY_SHADER);
	else
	{
		cout << "Unknown Shader Type.\n";
		return 0;
	}

	FILE* fp = fopen(filename, "rb");
	fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);  //same as rewind(fp);

	char* source = (char*)malloc(sizeof(char) * (fsize + 1));
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
		cout << "ERROR::SHADER::" << type << "::COMPLIATION_FAILED\n" << infoLog << endl;
		return -1;
	}

	return shader;
}

unsigned int createProgram(unsigned int vertexShader, unsigned int geometryShader, unsigned int fragmentShader)
{
	unsigned int program = glCreateProgram();

	//Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, geometryShader);
	glAttachShader(program, fragmentShader);

	//Link our program
	glLinkProgram(program);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	int success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		int maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		char* infoLog = (char*)malloc(sizeof(char) * (maxLength));
		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);

		//We don't need the program anymore.
		glDeleteProgram(program);
		//Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		cout << "LINKING ERROR: ";
		puts(infoLog);
		free(infoLog);

		return -1;
	}

	//Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	return program;
}

unsigned int ModelVAO(Object* model)
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

void LoadTexture(unsigned int& texture, const char* tFileName) {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(tFileName, &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

glm::mat4 getPerspective()
{
	return glm::perspective(
		glm::radians(45.0f),
		(float)windowWidth / (float)windowHeight, 
		1.0f, 100.0f);
}

glm::mat4 getView()
{
	if (cameraMove == 0) // at first
		return glm::lookAt(
		glm::vec3(0, 4, 4),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));
	if (cameraMove == 1) // duck close up
		return glm::lookAt(
		glm::vec3(3, 1, 4),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));
	if (cameraMove == 3) // ball close up
		return glm::lookAt(
		glm::vec3(-1.5, 0.5, 3),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));
	if (cameraMove == 2) // last scene walk away
		return glm::lookAt(
		glm::vec3(-3, 1, 5),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));
}