#pragma once
#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform {
public:
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	Transform():model(1.0f), view(1.0f), projection(1.0f) {}
	Transform(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) {
		this->model = model;
		this->view = view;
		this->projection = projection;
	}

	// ÷ÿ–¬÷√Œª
	void reset() {
		model = glm::mat4(1.0f);
		view = model;
		projection = model;
	}
};

#endif // !TRANSFORMATION_H

