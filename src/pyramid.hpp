#ifndef PYRAMID_HPP
#define PYRAMID_HPP

#include "triangle.hpp"
#include "three_dim_mesh.hpp"

class Pyramid : public ThreeDimMesh, public ShaderWrap, public SpaceActionController {
public:
	Pyramid(const std::vector<GLfloat>& vertices) {
		ThreeDimMesh::Init(vertices, {
			0, 3, 1,
			1, 3, 2,
			2, 3, 0,
			0, 1, 2
		});
	}
	bool CompileShaders() override {
		shader_ = glCreateProgram();

		if (!shader_) {
			throw std::exception("Could not create a program!");
		}
		AddShadersToProgram(shader_);

		GLint result = 0;
		GLchar log[1024] = { 0 };

		glLinkProgram(shader_);
		glGetProgramiv(shader_, GL_LINK_STATUS, &result);
		if (!result) {
			glGetProgramInfoLog(shader_, sizeof(log), NULL, log);
			LOG(ERROR) << __FUNCTION__ << "]: " << log << std::endl;
			return false;
		}

		glValidateProgram(shader_);
		glGetProgramiv(shader_, GL_VALIDATE_STATUS, &result);
		if (!result) {
			glGetProgramInfoLog(shader_, sizeof(log), NULL, log);
			LOG(ERROR) << "[ERROR: " << __FUNCTION__ << "]: " << log << std::endl;
			return false;
		}

		uniform_model_ = glGetUniformLocation(shader_, "model");

		return true;
	}
	void Draw() {
		glUseProgram(shader_);

		//TODO: Implement caching
		glm::mat4 model(1.0f);
		Update(model);
		glUniformMatrix4fv(uniform_model_, 1, GL_FALSE, glm::value_ptr(model));;

		glBindVertexArray(vertex_array_object_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_boject_);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glUseProgram(0);
	}
protected:
	GLuint uniform_model_;
	GLuint shader_;
};

#endif