#ifndef _CAMERA_H
#define _CAMERA_H

#include "Shader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

class Camera {
	public: 
		Camera(GLFWwindow *window, GLfloat screenWidth, GLfloat screenHeight);
		glm::vec3 cameraPos, cameraFront, cameraUp;
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 standardMat;
		void moveOnce();
		void updateview();

		void (*MBcallback)(GLFWwindow* window, int button, int action, int mods);
		void (*Mcallback)(GLFWwindow* window, double xpos, double ypos);
		void (*Kcallback)(GLFWwindow* window, int key, int scancode, int action, int mode);

		void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


		int isMousePressed;
		GLfloat mouselastX, mouselastY;
		GLfloat deltaTime = 0.0f;   // 当前帧与上一帧的时间差
		GLfloat lastFrame = 0.0f;   // 上一帧的时间
		GLfloat yaw = -90.0f;
		GLfloat pitch = 0;
		bool keys[1024];
};

#endif