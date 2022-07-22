#include<MEP-3D/model_controller.hpp>
#include<MEP-3D/utils.hpp>
#include<glm/gtx/string_cast.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/glm.hpp>
#include<vector>
#include<glog/logging.h>

ModelAction::ModelAction(Type type) : type_(type) {}

ModelAction::Type ModelAction::GetType() const {
	return type_;
}

ModelController::ModelController() {
	LOG(INFO) << __FUNCTION__;
	ResetModel();
	InitModelArray();
}

void ModelController::PushObjectAction(ModelActionPtr space_action) {
	space_actions_[space_action->GetType()].emplace(std::move(space_action));
}

void ModelController::Update() {
	for (auto& action : space_actions_) {
		if (!action.second.empty()) {
			auto temp_action = std::move(action.second.front());
			action.second.pop();
			temp_action->Update(model_);
		}
	}
}

void ModelController::ResetModel() {
	model_ = glm::mat4(1.0f);
}

glm::mat4& ModelController::GetModel() {
	return model_;
}

std::string ModelController::ToString() const {
	return "ObjectActionController: {\nmodel_matrix: " + glm::to_string(model_) + "\n}";
}

void ModelController::InitModelArray() {
	for (ModelAction::Type i = ModelAction::Transform; i != ModelAction::Count; i = utils::IncEnum(i)) {
		space_actions_[i] = std::queue<ModelActionPtr>();
	}
}

Transform::Transform(const Vec3f& val) : 
	x_(val.x_), y_(val.y_), z_(val.z_), ModelAction(Type::Transform) {}

Transform::Transform(float x, float y, float z) :
	x_(x), y_(y), z_(z), ModelAction(Type::Transform) {}

void Transform::Update(glm::mat4& model) {
	model = glm::translate(model, glm::vec3(x_, y_, z_));
}

Scale::Scale(const Vec3f& val) :
	x_(val.x_), y_(val.y_), z_(val.z_), ModelAction(Type::Scale) {}

Scale::Scale(float x, float y, float z) :
	x_(x), y_(y), z_(z), ModelAction(Type::Scale) {}

void Scale::Update(glm::mat4& model) {
	model = glm::scale(model, glm::vec3(x_, y_, z_));
}

Rotate::Rotate(float degrees, Axis axis) : ModelAction(Type::Rotate) {
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