#pragma once

#include <glm\vec3.hpp>

struct Bounds {
	glm::vec3 min;
	glm::vec3 max;

	void include(const glm::vec3& pos);
};