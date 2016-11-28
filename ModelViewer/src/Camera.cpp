#include "Camera.h"

#include <glm\vec4.hpp>
#include <glm\gtc\matrix_transform.hpp>

Projection::Projection(float width, float height, float near, float)
	: width(1.0f)
	, height(1.0f)
	, near(0.1f)
	, far(10.0f) {
}

Ortho::Ortho()
	: Projection(1, 1, -0.5, 0.5) {
}

Perspective::Perspective()
	: Projection(1, 1, 0.1f, 10.0f)
	, fov(glm::radians(60.0f)){
}

glm::mat4 Ortho::make_projection_mat() const {
	return glm::ortho(-width / 2, width / 2, height / 2, -height / 2, near, far);
}

glm::mat4 Perspective::make_projection_mat() const {
	return glm::perspectiveFov(fov, width, height, near, far);
}

Camera::Camera()
	: pos(0, 0 ,0)
	, target(0, 0, 1)
	, up(0, 1, 0) {
}

glm::mat4 Camera::make_view_mat() const {
	return glm::lookAt(pos, target, up);
}

