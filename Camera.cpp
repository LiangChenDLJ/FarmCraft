#include "Camera.h"

Camera::Camera(GLFWwindow *window, GLfloat screenWidth, GLfloat screenHeight) {
	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / screenHeight, 0.1f, 100.0f);

	isMousePressed = 0;
	mouselastX = screenWidth / 2, mouselastY = screenHeight / 2;
	yaw = -90.0f;
	pitch = 0;
};

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	GLfloat sensitivity = 0.1;

	GLfloat xoffset = xpos - mouselastX;
	GLfloat yoffset = mouselastY - ypos;
	mouselastX = xpos;
	mouselastY = ypos;
	if (!isMousePressed) return;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void Camera::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if (action == GLFW_PRESS && key == GLFW_KEY_R) {
		cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		yaw = -90.0f;
		pitch = 0.0f;
	}
	else if (action == GLFW_PRESS) {
		keys[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		keys[key] = false;
	}
	return;
}

void Camera::moveOnce() {
	// camera movement
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	GLfloat cameraSpeed = 2.5f * deltaTime;
	if (keys[GLFW_KEY_W])
		cameraPos += glm::normalize(glm::cross(glm::cross(cameraFront, cameraUp), cameraFront)) * cameraSpeed;
	if (keys[GLFW_KEY_S])
		cameraPos -= glm::normalize(glm::cross(glm::cross(cameraFront, cameraUp), cameraFront)) * cameraSpeed;
	if (keys[GLFW_KEY_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_Q])
		cameraPos += cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_E])
		cameraPos -= cameraSpeed * cameraFront;
	updateview();
}

void Camera::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		isMousePressed = 1;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		isMousePressed = 0;
	}
}

void Camera::updateview(){
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}


