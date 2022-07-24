#ifndef SPACE_ACTION_HPP
#define SPACE_ACTION_HPP

#include <iostream>
#include <memory>
#include <queue>
#include <unordered_map>

#include <glm/glm.hpp>

#include <MEP-3D/common.hpp>
#include <MEP-3D/vector.hpp>

class ModelAction;
using ModelActionPtr = std::unique_ptr<ModelAction>;

class ModelAction {
 public:
  enum Type { Transform = 0, Rotate = 1, Scale, Count };
  ModelAction(Type type);
  Type GetType() const;
  virtual void Update(glm::mat4& model) = 0;
  virtual ~ModelAction() = default;

 private:
  Type type_;
};

class ModelController {
 public:
  ModelController();
  void PushObjectAction(ModelActionPtr space_action);
  void Update();
  void ResetModel();
  glm::mat4& GetModel();
  std::string ToString() const;

 private:
  void InitModelArray();

  std::unordered_map<ModelAction::Type, std::queue<ModelActionPtr>>
      space_actions_;
  glm::mat4 model_;
};

class Transform : public ModelAction {
 public:
  Transform(const Vec3f& val);
  Transform(float x, float y, float z);
  void Update(glm::mat4& model) override;

 protected:
  float x_, y_, z_;
};

class Scale : public ModelAction {
 public:
  Scale(const Vec3f& val);
  Scale(float x, float y, float z);
  void Update(glm::mat4& model) override;

 protected:
  float x_, y_, z_;
};

class Rotate : public ModelAction {
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