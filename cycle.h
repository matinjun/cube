#pragma once
#ifndef CYCLE_H
#define CYCLE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Cycle {
private:
	float angle;
	int n;
	float state[4];

public:
	Cycle(float angle = glm::radians(15.0f)) {
		this->angle = angle;
		n = 0;
		state[0] = 1.0f;
		state[1] = -1.0f;
		state[2] = -1.0f;
		state[3] = 1.0f;
	}

	float operator() () {
		auto res = state[n++] * angle;
		if (n > 3) n = 0;
		return res;
	}

};

#endif // !CYCLE_H
