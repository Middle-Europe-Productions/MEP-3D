#ifndef SPACE_ACTION_HPP
#define SPACE_ACTION_HPP

#include<memory>
#include<vector>
#include<iostream>
#include<glm/glm.hpp>

#include "utils.hpp"
#include "common.hpp"

class SpaceAction;
using Space = glm::mat4;
using SpaceActionPtr = std::unique_ptr<SpaceAction>;

class SpaceAction {
public:
	virtual void Update(Space& model) = 0;
};

class SpaceActionController {
	std::vector<SpaceActionPtr> space_actions_;
public:
	void AddSpaceAction(SpaceActionPtr space_action) {
		space_actions_.emplace_back(std::move(space_action));
	}
	void Update(Space& model) {
		for (auto& action : space_actions_) {
			action->Update(model);
		}
	}
};

class Transform : public SpaceAction {
public:
	Transform(float x, float y, float z) :
		x_(x), y_(y), z_(z) {}
	void Update(Space& model) override {
		model = glm::translate(model, glm::vec3(x_, y_, z_));
	}
protected:
	float x_, y_, z_;
};

class Scale : public Transform {
public:
	Scale(float x, float y, float z) :
		Transform(x, y, z) {}
	void Update(Space& model) override {
		model = glm::scale(model, glm::vec3(x_, y_, z_));
	}
private:
};

class Rotate : public SpaceAction {
public:
	Rotate(float degrees, Axis axis = Axis::X) {
		SetRotationDeg(degrees, axis);
	}

	void SetRotationRad(float radians, Axis axis = Axis::X) {
		radians_ = radians;
		axis_ = axis;
	}
	void SetRotationDeg(float degrees, Axis axis = Axis::X) {
		radians_ = degrees * 3.14159265f / 180.0f;
		axis_ = axis;
	}
	void Update(Space& model) override {
		model = glm::rotate(model, radians_, utils::AxisToGlm(axis_));
	}
private:
	float radians_;
	Axis axis_;
};

#endif