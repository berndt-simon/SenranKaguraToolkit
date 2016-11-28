#pragma once

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>

struct Projection {
	Projection(float widht, float height, float near, float far);
	float width;
	float height;
	float near, far;
	virtual glm::mat4 make_projection_mat() const = 0;
};

struct Ortho : Projection {
	Ortho();
	glm::mat4 make_projection_mat() const override;
};

struct Perspective : Projection {
	Perspective();
	float fov;
	glm::mat4 make_projection_mat() const override;
};

struct Camera {
	Camera();

	glm::vec3 pos;
	glm::vec3 target;
	glm::vec3 up;

	glm::mat4 make_view_mat() const;
};