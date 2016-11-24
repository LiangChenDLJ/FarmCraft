// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// assimp (3D module input)
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include "Shader.h"
#include "GLmySphere.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "LightSource.h"
int screenWidth = 800;
int screenHeight = 600;

Camera* mycamera;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(int argc, char* argv[]) {
	int i;
	glfwInit();

	// windows initing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "FarmCraft", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// glew initing
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);

	// generate shadows 
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLfloat near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

	// initial shader
	Shader* myshader = new Shader("VertexShader.vts", "FragmentShader.fgs");

	mycamera = new Camera(window, screenWidth, screenHeight);

	// active kb and mouse control 
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);


	GLchar* nanosuitPath = "Model/nanosuit/nanosuit.obj";
	GLchar* housePath = "Model/house.obj";
	Model *myModel = new Model(nanosuitPath);
	LightSource *mylights[2];
	mylights[0] = new LightSource(glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	mylights[1] = new LightSource(glm::vec3(-1.7f, 0.9f, 1.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0, 0.09, 0.0032);

	myshader->Use();
	glUniformMatrix4fv(glGetUniformLocation(myshader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(mycamera->projection));
	while (!glfwWindowShouldClose(window))
	{
		// Check and call events
		glfwPollEvents();
		mycamera->moveOnce();
		// Clear the colorbuffer
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myshader->Use();
		//manage the camera
		
		glUniformMatrix4fv(glGetUniformLocation(myshader->Program, "view"), 1, GL_FALSE, glm::value_ptr(mycamera->view));
		// manage the light sources 
		glUniform3f(glGetUniformLocation(myshader->Program, "viewPos"), mycamera->cameraPos.x, mycamera->cameraPos.y, mycamera->cameraPos.z);
		for (i = 0; i < 2; i++) {
			mylights[i]->useLight(myshader, i);
		}
		// Draw the loaded model
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// It's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(glGetUniformLocation(myshader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		myModel->Draw(*myshader);

		// Swap the buffers
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
	mycamera->mouse_button_callback(window, button, action, mods);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	mycamera->mouse_callback(window, xpos, ypos);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	mycamera->key_callback(window, key, scancode, action, mode);
}