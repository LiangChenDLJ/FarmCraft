#include"Shader.h"


Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	// 1. 从文件路径中获取顶点/片段着色器
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// 保证ifstream对象可以抛出异常：
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	try
	{
		// 打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// 读取文件的缓冲内容到流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// 关闭文件
		vShaderFile.close();
		fShaderFile.close();
		// 转换流至GLchar数组
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	//std::cout << vShaderCode << std::endl << std::endl << fShaderCode << std::endl;

	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// Print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// fragment shader creating
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader Program
	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertex);
	glAttachShader(this->Program, fragment);
	glLinkProgram(this->Program);
	// Print linking errors if any
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Use() {
	glUseProgram(this->Program);
}
/* abandoned
void Shader::AssignUniform(const GLchar *name, GLchar *valtype, glm::mat4 *trans){
	if (valtype == "mf") {
		GLint uniformLoc = glGetUniformLocation(this->Program, name);
		glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(*trans));
	}
	else {
		std::cout << "AssignUniform_ERROE_Illegal_valtype";
	}
}

void Shader::AssignUniform(const GLchar *name, GLchar *valtype, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3){
	if (valtype == "f") {
		GLint uniformLoc = glGetUniformLocation(this->Program, name);
		glUniform4f(uniformLoc, v0, v1, v2, v3);
	}
	else {
		std::cout << "AssignUniform_ERROE_Illegal_valtype";
	}
}

void Shader::AssignUniform(const GLchar *name, GLchar *valtype, GLfloat v0, GLfloat v1, GLfloat v2) {
	if (valtype == "f") {
		GLint uniformLoc = glGetUniformLocation(this->Program, name);
		glUniform3f(uniformLoc, v0, v1, v2);
	}
	else {
		std::cout << "AssignUniform_ERROE_Illegal_valtype";
	}
}
*/

