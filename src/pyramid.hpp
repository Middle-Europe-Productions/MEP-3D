#ifndef PYRAMID_HPP
#define PYRAMID_HPP

#include "triangle.hpp"
#include "three_dim_mesh.hpp"

class Pyramid : public ThreeDimMesh, public ShaderBinder, public SpaceActionController {
public:
	Pyramid(const std::vector<GLfloat>& vertices) {
		ThreeDimMesh::Init(vertices, {
			0, 3, 1,
			1, 3, 2,
			2, 3, 0,
			0, 1, 2
		});
	}

	void Draw() {
		if (!Get()) {
			return;
		}
		ShaderBase* shader = Get();

		//TODO: Implement caching
		glm::mat4 model(1.0f);
		Update(model);
		glUniformMatrix4fv(shader->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(vertex_array_object_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_boject_);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

};

#endif