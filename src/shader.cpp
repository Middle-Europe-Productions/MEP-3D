#include<glm/gtc/type_ptr.hpp>
#include<MEP-3D/shader.hpp>
#include<glog/logging.h>

bool ShaderBase::IsCompiled() const {
	return status_;
}

void ShaderBase::SetStatus(bool status) {
	status_ = status;
}


void ShaderBinder::Bind(ShaderBase* shader) {
	shader_ = shader;
}

void ShaderBinder::UnBind() {
	shader_ = nullptr;
}

ShaderBase* ShaderBinder::Get() {
	if (!shader_ || !shader_->IsCompiled())
		return nullptr;
	return shader_;
}

Shader::Shader() {
	shader_id_ = 0;
	uniform_projection_ = 0;
	uniform_model_ = 0;
	uniform_view_ = 0;
}
bool Shader::Compile(const std::string& vertex_code, const std::string& fragment_code) {
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

void Shader::StartUsing() {
	glUseProgram(shader_id_);
}

void Shader::StopUsing() {
	glUseProgram(0);
}

bool Shader::CreateFromFile(const std::string& vertex_location, const std::string& fragment_location) {
	std::string vertex_code = Shader::LoadFromFile(vertex_location);
	std::string fragment_code = Shader::LoadFromFile(fragment_location);
	if (vertex_code == "" || fragment_code == "")
		return false;
	return CreateFromString(vertex_code, fragment_code);
}

bool Shader::CreateFromString(const std::string& vertex_code, const std::string& fragment_code) {
	return Compile(vertex_code, fragment_code);
}

void Shader::Clear() {
	if (shader_id_) {
		glDeleteProgram(shader_id_);
		shader_id_ = 0;
	}
	uniform_view_ = 0;
	uniform_model_ = 0;
	uniform_projection_ = 0;
	SetStatus(false);
}

GLuint Shader::GetProjectionLocation() {
	return uniform_projection_;
}

GLuint Shader::GetModelLocation() {
	return uniform_model_;
}

GLuint Shader::GetViewLocation() {
	return uniform_view_;
}

bool Shader::SetUniformExt(GLuint uniform_location, const glm::mat4& matrix) {
	glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(matrix));
	return true;
}

bool Shader::SetUniformExt(GLuint uniform_location, float value) {
	glUniform1f(uniform_location, value);
	return true;
}

bool Shader::SetUniformExt(GLuint uniform_location, int value) {
	glUniform1i(uniform_location, value);
	return true;
}

bool Shader::SetUniform(const std::string& name, const glm::mat4& matrix) {
	GLuint uni_location = glGetUniformLocation(shader_id_, name.c_str());
	if (!GetUniformLocation(uni_location, name)) {
		return false;
	}
	glUniformMatrix4fv(uni_location, 1, GL_FALSE, glm::value_ptr(matrix));
	return true;
}

bool Shader::SetUniform(const std::string &name, float value) {
	GLuint uni_location = glGetUniformLocation(shader_id_, name.c_str());
	if (!GetUniformLocation(uni_location, name)) {
		return false;
	}
	glUniform1f(uni_location, value);
	return true;
}

bool Shader::SetUniform(const std::string& name, int value) {
	GLuint uni_location = glGetUniformLocation(shader_id_, name.c_str());
	if (!GetUniformLocation(uni_location, name)) {
		return false;
	}
	glUniform1i(uni_location, value);
	return true;
}

std::string Shader::LoadFromFile(const std::string& path) {
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

Shader::~Shader() {
	Clear();
}

bool Shader::AddShader(GLuint program, const std::string& shader_code, GLenum shader_type) {
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

bool Shader::GetUniformLocation(GLuint &location, const std::string &name) {
	GLuint local_loc = glGetUniformLocation(shader_id_, name.c_str());
	if (local_loc == -1) {
		LOG(ERROR) << "Value: " + name + " does not exist in shader id: " + std::to_string(GetId());
		return false;
	}
	location = local_loc;
	return true;
}