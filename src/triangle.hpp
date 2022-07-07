#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include<vector>
#include<string>
#include<exception>
#include<memory>

#include<iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "drawable.hpp"
#include "space_action.hpp"
#include "shader.hpp"
#include "two_dim_mesh.hpp"

class Triangle: public TwoDimMesh, public ShaderBinder, public SpaceActionController {
public:
	Triangle(const std::vector<GLfloat>& vertices) {
		Init(vertices);
	}
	void Draw() override {
		if (!Get()) {
			return;
		}
		ShaderBase* shader = Get();
		//TODO: Implement caching
		glm::mat4 model(1.0f);
		Update(model);
		glUniformMatrix4fv(shader->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(model));;

		glBindVertexArray(vertex_array_object_);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}
};

#endif