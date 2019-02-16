#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "my_shader.h"
#include "stb_image.h"
#include "my_camera.h"

#include <iostream>
using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;

float alphaVal = 0.2f;
int cubeNum = 10;

float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePos[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

//MyCamera cam = MyCamera(glm::vec3(.0f, .0f, 3.f), glm::vec3(.0f, .0f, .0f));
MyCamera cam = MyCamera(glm::vec3(.0f, .0f, 3.f), .0f, -90.f);
float camSpeed = 2.5f;
double lastXPos, lastYpos;
bool mouseFistTime = true;
double deltaTime, lastFrameTime;
float MAX_PITCH = 50.f;


void framebufferCallback(GLFWwindow*, int, int);
void mouseCallback(GLFWwindow*, double, double);
void inputProcessHandler(GLFWwindow*);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "create window fail!" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// process func address with glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "glad fail!" << endl;
		glfwTerminate();
		return -1;
	}

	// set frame_buffer callback
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetFramebufferSizeCallback(window, framebufferCallback);

	// build and compile our shader program
	// ------------------------------------
	// vertex shader program
	MyShader shaderOrange = MyShader(
		"..\\shaders\\texture_triangle_vertex.vs",
		"..\\shaders\\texture_triangle_fragment.fs"
	);

	// generate textures
	GLuint textures[2];
	glGenTextures(2, textures);


	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;
	unsigned char* imageData1 = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (imageData1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Error: FAILED TO LOAD TEXTURE \"container.jpg\"." << endl;
	}
	stbi_image_free(imageData1);


	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);

	unsigned char* imageData2 = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (imageData2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Error: FAILED TO LOAD TEXTURE awesomeface.png." << endl;
	}
	stbi_image_free(imageData2);
	
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// read translate position matrix
	glm::mat4* transMat4 = new glm::mat4[cubeNum];
	for (int i = 0; i < 10; i++)
	{
		glm::mat4 model;
		model = glm::translate(model, cubePos[i]);
		float _angle = 20.f * i;
		model = glm::rotate(model, glm::radians(_angle), glm::vec3(1.f, .3f, .5f));
		transMat4[i] = model;
	}
	GLuint TRANS_VBO;
	glGenBuffers(1, &TRANS_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, TRANS_VBO);
	glBufferData(GL_ARRAY_BUFFER, cubeNum * sizeof(glm::mat4), &transMat4[0], GL_STATIC_DRAW);
	// passing 4 * vec4 instead of mat4
	int attrOffset = 2;
	for (int i = 0; i < 4; i++)
	{
		glVertexAttribPointer(i + attrOffset, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(i * sizeof(glm::vec4)));
		glEnableVertexAttribArray(i + attrOffset);
		glVertexAttribDivisor(i + attrOffset, 1);
	}

	// clear
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);

	shaderOrange.use();
	shaderOrange.setInt("texture1", 0);
	shaderOrange.setInt("texture2", 1);

	glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)WIDTH / (float)HEIGHT, .1f, 100.f);
	shaderOrange.setMaxtrix("projection", glm::value_ptr(projection));

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		double curFrameTime = glfwGetTime();
		deltaTime = curFrameTime - lastFrameTime;
		lastFrameTime = curFrameTime;
		inputProcessHandler(window);

		glClearColor(0.5f, 0.5f, 0.5f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);

		shaderOrange.use();
		shaderOrange.setFloat("alpha", alphaVal);
		glm::mat4 view;
		view = cam.getViewMat4();
		shaderOrange.setMaxtrix("view", glm::value_ptr(view));

		glBindVertexArray(VAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 36, cubeNum);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// terminate
	glfwTerminate();
	return 0;
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	//cout << xPos << "\t" << yPos << endl;
	if (mouseFistTime == true)
	{
		lastXPos = xPos;
		lastYpos = yPos;
		mouseFistTime = false;
		cout << xPos << "\t" << yPos << endl;
		return;
	}

	double deltaX = (xPos - lastXPos) * deltaTime; // yaw
	double deltaY = (lastYpos - yPos) * deltaTime; // pitch
	float yaw = cam.getYaw() + deltaX;
	float pitch = cam.getPitch() + deltaY;
	if (pitch > MAX_PITCH)
		pitch = MAX_PITCH;
	else if (pitch < -MAX_PITCH)
		pitch = -MAX_PITCH;

	cam.updateCamDirection(pitch, yaw);
	lastXPos = xPos;
	lastYpos = yPos;

}

void framebufferCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void inputProcessHandler(GLFWwindow* window)
{
	// close application
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	// change opacity of texture
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		alphaVal += 0.01f;
		if (alphaVal > 1.f)
		{
			alphaVal = 1.f;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		alphaVal -= 0.01f;
		if (alphaVal < 0.f)
		{
			alphaVal = 0.f;
		}
	}

	// handle moving camera
	float realSpeed = camSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cam.updateCamPosition(GLFW_KEY_W, realSpeed);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cam.updateCamPosition(GLFW_KEY_S, realSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cam.updateCamPosition(GLFW_KEY_A, realSpeed);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cam.updateCamPosition(GLFW_KEY_D, realSpeed);
	}

}