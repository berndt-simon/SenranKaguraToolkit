#include "CameraControls.h"

#include <glm\common.hpp>
#include <glm\vec4.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\polar_coordinates.hpp>

FlyView::FlyView(Camera& camera) 
	: _camera(camera) 
	, _linear_speed(16.0f) {
}

void FlyView::start_movement(Direction direction, bool enable) {
	_movement.set(direction, enable);
}

void FlyView::stop_movement(Direction direction) {
	_movement.reset(direction);
}

void FlyView::stop_movement() {
	_movement.reset();
}

void FlyView::update(float dt) {
	glm::vec3 dir(0);
	if (_movement[Forward]) {
		dir.z += _linear_speed;
	}
	if (_movement[Backward]) {
		dir.z -= _linear_speed;
	}
	if (_movement[Left]) {
		dir.x -= _linear_speed;
	}
	if (_movement[Right]) {
		dir.x += _linear_speed;
	}
	if (_movement[Up]) {
		dir.y += _linear_speed;
	}
	if (_movement[Down]) {
		dir.y -= _linear_speed;
	}
	dir = glm::normalize(dir);
}

float FlyView::linear_speed() const {
	return _linear_speed;
}

float& FlyView::linear_speed() {
	return _linear_speed;
}

OrbitView::OrbitView(Camera& camera)
	: _camera(camera)
	, orbit_radius(1.0f)
	, latitude(0.0f)
	, longitude(0.0f) {
}

void OrbitView::apply() {
	while (longitude < glm::pi<float>()) {
		longitude += 2 * glm::pi<float>();
	}
	while (longitude > glm::pi<float>()) {
		longitude -= 2 * glm::pi<float>();
	}
	latitude = glm::clamp<float>(latitude, -glm::radians(85.0f), glm::radians(85.0f));

	_camera.pos = glm::euclidean(glm::vec2(latitude, longitude)) * orbit_radius;
}
