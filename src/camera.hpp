#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <unordered_map>
#include <memory>

#include "updatable.hpp"
#include "window_observer.hpp"
#include "key_event.hpp"
#include "time_delta.hpp"

#include "glm/gtx/string_cast.hpp"
#include <string>
struct CameraControls {
	CameraControls() :
		up(Keyboard::W),
		down(Keyboard::S),
		left(Keyboard::A),
		right(Keyboard::D) {}
	Keyboard up;
	Keyboard down;
	Keyboard left;
	Keyboard right;
};

struct CameraConfig {
	glm::vec3 start_position;
	glm::vec3 start_up;
	GLfloat start_yaw;
	GLfloat start_pitch;
	GLfloat start_move_speed;
	GLfloat start_turn_speed;
};

class Camera : public Updatable, public WindowObserver {
public:
	Camera(CameraConfig config, CameraControls controls = CameraControls()) :
		controls_(controls),
		camera_time_delta_(TimeDelta::GetInstance()) {
		position_ = config.start_position;
		world_up_ = config.start_up;
		yaw_ = config.start_yaw;
		pitch_ = config.start_pitch;
		front_ = glm::vec3(0.0f, 0.0f, -1.0f);
		move_speed_ = config.start_move_speed;
		turn_speed_ = config.start_turn_speed;
		last_mouse_x_ = 0.f;
		last_mouse_y_ = 0.f;
		mouse_x_change = 0.f;
		mouse_y_change = 0.f;
		initial_move_ = true;
		Update();
	}
	void Update() override {
		ValidateKeyboardInput();
		UpdateInt([this]() {
			front_.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
			front_.y = sin(glm::radians(pitch_));
			front_.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
			front_ = glm::normalize(front_);

			right_ = glm::normalize(glm::cross(front_, world_up_));
			up_ = glm::normalize(glm::cross(right_, front_));
			});
	}
	glm::mat4 GetViewMatrix() const {
		return glm::lookAt(position_, position_ + front_, up_);
	}
	virtual void OnKeyEvent(KeyEvent event) {
		if (event.action == Action::Pressed || event.action == Action::Released) {
			if (event.code == controls_.up ||
				event.code == controls_.down ||
				event.code == controls_.left ||
				event.code == controls_.right) {
				key_status_[event.code] = event.action == Action::Pressed;
			}
		}
	}
	virtual void OnMouseEvent(MouseEvent event) {
		if (initial_move_) {
			last_mouse_x_ = event.x;
			last_mouse_y_ = event.y;
			initial_move_ = false;
		}
		else if (last_mouse_x_ == event.x &&
				 last_mouse_y_ == event.y) {
			return;
		}
		mouse_x_change = event.x - last_mouse_x_;
		mouse_y_change = event.y - last_mouse_y_;

		last_mouse_x_ = event.x;
		last_mouse_y_ = event.y;

		mouse_x_change *= turn_speed_;
		mouse_y_change *= turn_speed_;
		yaw_ += mouse_x_change;
		pitch_ += mouse_y_change;
		if (pitch_ > 89.9f) {
			pitch_ = 89.9f;
		}
		if (pitch_ > -89.9f) {
			pitch_ = -89.9f;
		}
		Changed();
	}
	std::string ToString() const {
		return std::string(
				"Position: " + glm::to_string(position_) + ", \n" +
				"Front: " + glm::to_string(front_) + ", \n" +
				"Up: " + glm::to_string(up_) + ", \n" +
				"Right: " + glm::to_string(right_) + ", \n" +
				"World_up: " + glm::to_string(world_up_) + ", \n" +
				"Yaw: " + std::to_string(yaw_) + ", \n" +
				"Pitch: " + std::to_string(pitch_));
	}
private:
	void InitKeyboardMap() {
		key_status_[controls_.up] = false;
		key_status_[controls_.down] = false;
		key_status_[controls_.left] = false;
		key_status_[controls_.right] = false;
	}
	void ValidateKeyboardInput() {
		bool pressed = false;
		double velocity = move_speed_ * camera_time_delta_->GetTimeDelta();
		if (key_status_[controls_.up]) {
			position_ += front_ * (GLfloat)velocity;
			pressed = true;
		}
		if (key_status_[controls_.down]) {
			position_ -= front_ * (GLfloat)velocity;
			pressed = true;
		}
		if (key_status_[controls_.left]) {
			position_ -= right_ * (GLfloat)velocity;
			pressed = true;
		}
		if (key_status_[controls_.right]) {
			position_ += right_ * (GLfloat)velocity;
			pressed = true;
		}
		if (pressed)
			Changed();
	}
	std::unordered_map<Keyboard, bool> key_status_;
	CameraControls controls_;
	TimeDeltaPtr camera_time_delta_;
	glm::vec3 position_;
	glm::vec3 front_;
	glm::vec3 up_;
	glm::vec3 right_;
	glm::vec3 world_up_;

	GLfloat yaw_;
	GLfloat pitch_;

	GLfloat move_speed_;
	GLfloat turn_speed_;

	GLfloat last_mouse_x_;
	GLfloat last_mouse_y_;
	GLfloat mouse_x_change;
	GLfloat mouse_y_change;
	bool initial_move_;
};

#endif