#ifndef SHADER_WRAP_HPP
#define SHADER_WRAP_HPP

#include <vector>
#include <iostream>
#include <GL/glew.h>

class ShaderWrap {
public:
	using ShadersContainer = std::vector<std::pair<std::string, GLenum>>;
	void AddShader(const std::string& shader, GLenum shaderType) {
		shader_container_.push_back({ shader, shaderType });
	}
	void AddShaders(const ShadersContainer& shaders) {
		for (auto& shader : shaders) {
			shader_container_.push_back(shader);
		}
	}
	virtual bool CompileShaders() = 0;
protected:
	int AddShadersToProgram(GLuint program) {
		int complete = 0;
		for (auto& shader : shader_container_) {
			GLuint gl_shader = glCreateShader(shader.second);

			const GLchar* code[1] = { shader.first.c_str() };
			GLint code_lenght[1] = { shader.first.size() };

			glShaderSource(gl_shader, 1, code, code_lenght);
			glCompileShader(gl_shader);

			GLint result = 0;
			GLchar log[1024] = { 0 };
			glGetShaderiv(gl_shader, GL_COMPILE_STATUS, &result);
			if (!result) {
				glGetShaderInfoLog(gl_shader, 1024, NULL, log);
				std::cout << "[ERROR: " << __FUNCTION__ << "]: " << log << std::endl;
			}
			else {
				complete++;
				glAttachShader(program, gl_shader);
			}
		}
		//DCHECK(complete == shader_container_.size());
		if (complete != shader_container_.size()) {
			std::cout << "[WARNING: " << __FUNCTION__ << "]: "
				<< " Could not compile all shaders: " << shader_container_.size() - complete
				<< ", out of: " << shader_container_.size() << std::endl;
		}
		return complete;
	}
	ShadersContainer shader_container_;
};

#endif