#ifndef PYRAMID_HPP
#define PYRAMID_HPP

#include<MEP-3D/asset_controller.hpp>
#include<MEP-3D/object_action.hpp>
#include<MEP-3D/mesh.hpp>
#include<MEP-3D/vector.hpp>

class Pyramid : public Mesh {
public:
	Pyramid(Vec3f initial_position = { 0.0f, 0.0f, 0.0f });
};

#endif