#include "LightSource.h"

LightSource::LightSource(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
	this->isPoint = TYPE_PARALLEL;
	this->position = position;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}
LightSource::LightSource(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
	float constant, float linear, float quadratic) {
	this->isPoint = TYPE_POINT;
	this->position = position;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

int LightSource::useLight(Shader* shader, int number) {
	std::string snumber = std::to_string(number);
	std::string basestr("Lights[");
	glUniform1i(glGetUniformLocation(shader->Program, (basestr + snumber + "].isPoint").c_str()), isPoint ? 1 : 0);
	if (isPoint == TYPE_POINT) {
		glUniform1f(glGetUniformLocation(shader->Program, (basestr + snumber + "].constant").c_str()), constant);
		glUniform1f(glGetUniformLocation(shader->Program, (basestr + snumber + "].linear").c_str()), linear);
		glUniform1f(glGetUniformLocation(shader->Program, (basestr + snumber + "].quadratic").c_str()), quadratic);
	}
	//std::cout << ambient.x << "\t" << ambient.y << "\t" << ambient.z << "\n";
	glUniform3f(glGetUniformLocation(shader->Program, (basestr + snumber + "].position").c_str()), position.x, position.y, position.z);
	glUniform3f(glGetUniformLocation(shader->Program, (basestr + snumber + "].ambient").c_str()), ambient.x, ambient.y, ambient.z);
	glUniform3f(glGetUniformLocation(shader->Program, (basestr + snumber + "].diffuse").c_str()), diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(glGetUniformLocation(shader->Program, (basestr + snumber + "].specular").c_str()), specular.x, specular.y, specular.z);
	return 0;
}