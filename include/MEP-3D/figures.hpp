#ifndef PYRAMID_HPP
#define PYRAMID_HPP

#include <MEP-3D/asset_controller.hpp>
#include <MEP-3D/mesh.hpp>
#include <MEP-3D/model_controller.hpp>
#include <MEP-3D/vector.hpp>

class Pyramid : public Mesh {
 public:
  Pyramid(Vec3f initial_position = {0.0f, 0.0f, 0.0f});
};

class Plane : public Mesh {
 public:
  Plane(float size = 10.0f, Vec3f initial_position = {0.0f, 0.0f, 0.0f});
};

class Cube : public Mesh {
 public:
  Cube(float size = 10.0f, Vec3f initial_position = {0.0f, 0.0f, 0.0f});
};

class Triangle : public Mesh {
 public:
  Triangle(Vec3f initial_position = {0.0f, 0.0f, 0.0f});
};

#endif