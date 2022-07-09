#ifndef SHADER_HPP
#define SHADER_HPP

#include<string>
#include <fstream>
#include<GL/glew.h>

class ShaderBase {
public:
	ShaderBase() : status_(false) {}

	virtual bool Compile(const std::string&, const std::string&) = 0;
	bool IsCompiled() const {
		return status_;
	}
	virtual GLuint GetProjectionLocation() = 0;
	virtual GLuint GetModelLocation() = 0;
	virtual GLuint GetViewLocation() = 0;
	virtual ~ShaderBase() = default;

protected:
	void SetStatus(bool status) {
		status_ = status;
	}

private:
	bool status_;
};

class ShaderBinder {
public:
	void Bind(ShaderBase* shader) {
		shader_ = shader;
	}
	void UnBind() {
		shader_ = nullptr;
	}
protected:
	ShaderBase* Get() {
		if (!shader_ || !shader_->IsCompiled())
			return nullptr;
		return shader_;
	}

private:
	ShaderBase* shader_;
};

class Shader : public ShaderBase {
public:
	Shader() {
		shader_id_ = 0;
		uniform_projection_ = 0;
		uniform_model_ = 0;
		uniform_view_ = 0;
	}
	bool Compile(const std::string& vertex_code, const std::string& fragment_code) override {
		SetStatus(false);
		shader_id_ = glCreateProgram();

		if (!shader_id_) {
			LOG(ERROR) << "Could not create a program!";
			return false;
		}
		if (!AddShader(shader_id_, vertex_code, GL_VERTEX_SHADER) ||
			!AddShader(shader_id_, fragment_code, GL_FRAGMENT_SHADER)) {
			return false;
		}

		GLint result = 0;
		GLchar log[1024] = { 0 };

		glLinkProgram(shader_id_);
		glGetProgramiv(shader_id_, GL_LINK_STATUS, &result);
		if (!result) {
			glGetProgramInfoLog(shader_id_, sizeof(log), NULL, log);
			LOG(ERROR) << __FUNCTION__ << "]: " << log << std::endl;
			return false;
		}

		glValidateProgram(shader_id_);
		glGetProgramiv(shader_id_, GL_VALIDATE_STATUS, &result);
		if (!result) {
			glGetProgramInfoLog(shader_id_, sizeof(log), NULL, log);
			LOG(ERROR) << "[ERROR: " << __FUNCTION__ << "]: " << log << std::endl;
			return false;
		}

		uniform_projection_ = glGetUniformLocation(shader_id_, "projection");
		uniform_model_ = glGetUniformLocation(shader_id_, "model");
		uniform_view_ = glGetUniformLocation(shader_id_, "view");

		SetStatus(true);
		return true;
	}

	void StartUsing() {
		glUseProgram(shader_id_);
	}

	void StopUsing() {
		glUseProgram(0);
	}

	bool CreateFromFile(const std::string& vertex_location, const std::string& fragment_location) {
		std::string vertex_code = Shader::LoadFromFile(vertex_location);
		std::string fragment_code = Shader::LoadFromFile(fragment_location);
		if (vertex_code == "" || fragment_code == "")
			return false;
		return CreateFromString(vertex_code, fragment_code);
	}

	bool CreateFromString(const std::string& vertex_code, const std::string& fragment_code) {
		return Compile(vertex_code, fragment_code);
	}

	void Clear() {
		if (shader_id_) {
			glDeleteProgram(shader_id_);
			shader_id_ = 0;
		}
		uniform_view_ = 0;
		uniform_model_ = 0;
		uniform_projection_ = 0;
		SetStatus(false);
	}

	GLuint GetProjectionLocation() override {
		return uniform_projection_;
	}

	GLuint GetModelLocation() override {
		return uniform_model_;
	}

	GLuint GetViewLocation() override {
		return uniform_model_;
	}

	static std::string LoadFromFile(const std::string& path) {
		std::ifstream file_stream(path, std::ios::in);
		if (!file_stream.is_open()) {
			LOG(ERROR) << "Could not open a file!";
			return "";
		}
		std::string output;
		std::string line = "";
		while (!file_stream.eof()) {
			std::getline(file_stream, line);
			output.append(line + "\n");
		}
		return output;
	}

	virtual ~Shader() {
		Clear();
	}

protected:
	GLuint shader_id_;
	GLuint uniform_projection_;
	GLuint uniform_model_;
	GLuint uniform_view_;

private:
	bool AddShader(GLuint program, const std::string& shader_code, GLenum shader_type) {
		GLuint gl_shader = glCreateShader(shader_type);

		const GLchar* code[1] = { shader_code.c_str() };
		GLint code_lenght[1] = { shader_code.size() };

		glShaderSource(gl_shader, 1, code, code_lenght);
		glCompileShader(gl_shader);

		GLint result = 0;
		GLchar log[1024] = { 0 };
		glGetShaderiv(gl_shader, GL_COMPILE_STATUS, &result);
		if (!result) {
			glGetShaderInfoLog(gl_shader, 1024, NULL, log);
			LOG(ERROR) << __FUNCTION__ << log << std::endl;
			return false;
		}
		glAttachShader(program, gl_shader);
		return true;
	}
};

#endif