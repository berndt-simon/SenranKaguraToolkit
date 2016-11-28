#pragma once

const float fps_update_interval(0.5f);

struct frame_counter_t {
	int frames;
	float last_update;
};

void update_frame_counter(float current_time, frame_counter_t& frame_counter, float& fps) {
	++frame_counter.frames;
	if (current_time - frame_counter.last_update >= fps_update_interval) {
		float fps_delta(current_time - frame_counter.last_update);
		fps = static_cast<float>(frame_counter.frames) / fps_delta;
		frame_counter.frames = 0;
		frame_counter.last_update += fps_update_interval;
	}
}