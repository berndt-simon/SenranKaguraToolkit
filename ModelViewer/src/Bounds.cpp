#include "Bounds.h"
#include <glm\common.hpp>

void Bounds::include(const glm::vec3& pos) {
	min = glm::min(min, pos);
	max = glm::max(max, pos);
}
