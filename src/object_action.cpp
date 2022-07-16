#include<MEP-3D/object_action.hpp>
#include<MEP-3D/utils.hpp>
#include<glm/gtx/string_cast.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/glm.hpp>

ObjectAction::ObjectAction(Type type) : type_(type) {}

ObjectAction::Type ObjectAction::GetType() const {
	return type_;
}

ObjectActionController::ObjectActionController() {
	ResetModel();
	InitObjectActionArray();
}

void ObjectActionController::PushObjectAction(ObjectActionPtr space_action) {
	space_actions_[space_action->GetType()].emplace(std::move(space_action));
}

void ObjectActionController::Update() {
	for (auto& action : space_actions_) {
		if (!action.second.empty()) {
			auto temp_action = std::move(action.second.front());
			action.second.pop();
			temp_action->Update(model_);
		}
	}
}

void ObjectActionController::ResetModel() {
	model_ = glm::mat4(1.0f);
}

glm::mat4& ObjectActionController::GetModel() {
	return model_;
}

std::string ObjectActionController::ToString() const {
	return "ObjectActionController: {\nmodel_matrix: " + glm::to_string(model_) + "\n}";
}

void ObjectActionController::InitObjectActionArray() {
	for (ObjectAction::Type i = ObjectAction::Transform; i != ObjectAction::Count; i = utils::IncEnum(i)) {
		space_actions_[i] = std::queue<ObjectActionPtr>();
	}
}

Transform::Transform(float x, float y, float z) :
	x_(x), y_(y), z_(z), ObjectAction(Type::Transform) {}

void Transform::Update(glm::mat4& model) {
	model = glm::translate(model, glm::vec3(x_, y_, z_));
}

Scale::Scale(float x, float y, float z) :
	x_(x), y_(y), z_(z), ObjectAction(Type::Scale) {}

void Scale::Update(glm::mat4& model) {
	model = glm::scale(model, glm::vec3(x_, y_, z_));
}

Rotate::Rotate(float degrees, Axis axis) : ObjectAction(Type::Rotate) {
	SetRotationDeg(degrees, axis);
}

void Rotate::SetRotationRad(float radians, Axis axis) {
	radians_ = radians;
	axis_ = axis;
}
void Rotate::SetRotationDeg(float degrees, Axis axis) {
	radians_ = degrees * 3.14159265f / 180.0f;
	axis_ = axis;
}
void Rotate::Update(glm::mat4& model) {
	model = glm::rotate(model, radians_, utils::AxisToGlm(axis_));
}