#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light {
public:
	glm::vec3 lightPos;
	glm::vec3 lightColor;
	Light(glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f), glm::vec3 lightColor = glm::vec3(1.0f)) {
		this->lightPos = lightPos;
		this->lightColor = lightColor;
	}
};

#endif // LIGHT_H

