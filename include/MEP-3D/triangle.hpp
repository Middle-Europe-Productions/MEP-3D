#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include<vector>
#include<string>
#include<exception>
#include<memory>

#include<iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<MEP-3D/drawable.hpp>
#include<MEP-3D/object_action.hpp>
#include<MEP-3D/shader.hpp>
#include<MEP-3D/two_dim_mesh.hpp>

class Triangle: public TwoDimMesh, public ShaderBinder, public ObjectActionController {
public:
	Triangle(const std::vector<GLfloat>& vertices);
	void Draw(RenderTarget& render_target) override;
};

#endif