#ifndef PYRAMID_HPP
#define PYRAMID_HPP

#include<MEP-3D/asset_controller.hpp>
#include<MEP-3D/object_action.hpp>
#include<MEP-3D/mesh.hpp>

class Pyramid : public Mesh, public AssetController, public ObjectActionController {
public:
	Pyramid(const std::vector<GLfloat>& vertices);
	void Draw(RenderTarget& render_target) override;
	std::string ToString() const;
};

#endif