#ifndef PYRAMID_HPP
#define PYRAMID_HPP

#include<MEP-3D/triangle.hpp>
#include<MEP-3D/mesh.hpp>

class Pyramid : public Mesh, public ShaderBinder, public ObjectActionController {
public:
	Pyramid(const std::vector<GLfloat>& vertices);
	void Draw(RenderTarget& render_target) override;
};

#endif