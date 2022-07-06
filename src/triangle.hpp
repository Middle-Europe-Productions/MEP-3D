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
#include "shader_wrap.hpp"
#include "two_dim_mesh.hpp"

class Triangle: public Drawable, public TwoDimMesh, public ShaderWrap, public SpaceActionController {
public:
	Triangle(const std::vector<GLfloat>& vertices) {
		Init(vertices);
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
			LOG(ERROR) << "[ERROR: " << __FUNCTION__ << "]: " << log << std::endl;
			return false;
		}

		glValidateProgram(shader_);
		glGetProgramiv(shader_, GL_VALIDATE_STATUS, &result);
		if (!result) {
			glGetProgramInfoLog(shader_, sizeof(log), NULL, log);
			LOG(ERROR) << __FUNCTION__ << "]: " << log << std::endl;
			return false;
		}

		uniform_model_ = glGetUniformLocation(shader_, "model");

		return true;
	}
	void Draw() override {
		glUseProgram(shader_);

		//TODO: Implement caching
		glm::mat4 model(1.0f);
		Update(model);
		glUniformMatrix4fv(uniform_model_, 1, GL_FALSE, glm::value_ptr(model));;

		glBindVertexArray(vertex_array_object_);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glUseProgram(0);
	}

protected:
	GLuint uniform_model_;
	GLuint shader_;
};

#endif