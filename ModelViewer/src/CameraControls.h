#pragma once

#include "Camera.h"
#include <bitset>

class FlyView {
public:
	enum Direction {
		Forward, Backward, Left, Right, Up, Down
	};

	FlyView(Camera& camera);
	~FlyView() = default;

	void start_movement(Direction direction, bool enable = true);
	void stop_movement(Direction direction);
	void stop_movement();

	void update(float dt);

	float linear_speed() const;
	float& linear_speed();

private:
	Camera& _camera;
	std::bitset<6> _movement;

	float _linear_speed;
};

class OrbitView {
public:
	OrbitView(Camera& camera);
	~OrbitView() = default;

	float orbit_radius;

	float latitude;
	float longitude;

	void apply();

private:
	Camera& _camera;
	
};