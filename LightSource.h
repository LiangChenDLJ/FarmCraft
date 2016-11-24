#pragma once
#ifndef _LIGHTSOURCE_H
#define _LIGHTSOURCE_H

#include<iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

class LightSource {
public:
	static const bool TYPE_POINT = true;
	static const bool TYPE_PARALLEL = false;
	LightSource(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	LightSource(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
		float constant, float linear, float quadratic);
	int useLight(Shader* shader, int number);
private:
	bool isPoint;
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

#endif