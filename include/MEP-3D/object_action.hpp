#ifndef SPACE_ACTION_HPP
#define SPACE_ACTION_HPP

#include<memory>
#include<unordered_map>
#include<queue>
#include<iostream>

#include<glm/glm.hpp>

#include<MEP-3D/common.hpp>
#include<MEP-3D/vector.hpp>

class ObjectAction;
using ObjectActionPtr = std::unique_ptr<ObjectAction>;

class ObjectAction {
public:
	enum Type {
		Transform = 0,
		Rotate = 1,
		Scale,
		Count
	};
	ObjectAction(Type type);
	Type GetType() const;
	virtual void Update(glm::mat4& model) = 0;
	virtual ~ObjectAction() = default;

private:
	Type type_;
};

class ObjectActionController {
public:
	ObjectActionController();
	void PushObjectAction(ObjectActionPtr space_action);
	void Update();
	void ResetModel();
	glm::mat4& GetModel();
	std::string ToString() const;

private:
	void InitObjectActionArray();

	std::unordered_map<ObjectAction::Type, std::queue<ObjectActionPtr>> space_actions_;
	glm::mat4 model_;
};

class Transform : public ObjectAction {
public:
	Transform(const Vec3f& val);
	Transform(float x, float y, float z);
	void Update(glm::mat4& model) override;

protected:
	float x_, y_, z_;
};

class Scale : public ObjectAction {
public:
	Scale(const Vec3f& val);
	Scale(float x, float y, float z);
	void Update(glm::mat4& model) override;

protected:
	float x_, y_, z_;
};

class Rotate : public ObjectAction {
public:
	Rotate(float degrees, Axis axis = Axis::X);
	void SetRotationRad(float radians, Axis axis = Axis::X);
	void SetRotationDeg(float degrees, Axis axis = Axis::X);
	void Update(glm::mat4& model) override;

private:
	float radians_;
	Axis axis_;

};

#endif