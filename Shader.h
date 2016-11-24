#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h> // ����glew����ȡ���еı���OpenGLͷ�ļ�

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
	public:
		// ����ID
		GLuint Program;
		// ��������ȡ��������ɫ��
		Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
		// ʹ�ó���
		void Use();
		/*abandoned
		void Shader::AssignUniform(const GLchar *name, GLchar *valtype, glm::mat4 *trans);

		void AssignUniform(const GLchar *name, GLchar *valtype, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

		void AssignUniform(const GLchar *name, GLchar *valtype, GLfloat v0, GLfloat v1, GLfloat v2);
		*/
};
#endif
