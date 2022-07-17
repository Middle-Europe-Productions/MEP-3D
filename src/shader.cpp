#include<glm/gtc/type_ptr.hpp>
#include<MEP-3D/shader.hpp>
#include<glog/logging.h>

Shader::Shader(): Identity(__FUNCTION__), status_(false) {
	LOG(INFO) << __FUNCTION__ << ", " << ToString();
	shader_id_ = 0;
}

bool Shader::Compile(const std::string& vertex_code, const std::string& fragment_code) {
	LOG(INFO) << __FUNCTION__ << ", " << ToString();
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
		LOG(ERROR) << __FUNCTION__ << "]: " << log;
		return false;
	}

	glValidateProgram(shader_id_);
	glGetProgramiv(shader_id_, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader_id_, sizeof(log), NULL, log);
		LOG(ERROR) << "[ERROR: " << __FUNCTION__ << "]: " << log;
		return false;
	}
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
	SetStatus(false);
}

bool Shader::SaveUniformToMemory(const std::string& name, unsigned int memory_id) {
	GLint uniform_value_loc = glGetUniformLocation(shader_id_, name.c_str());
	if (uniform_value_loc == -1) {
		LOG(ERROR) << "Could not find a variable!";
		return false;
	}
	uniform_memory_[memory_id] = uniform_value_loc;
	return true;
}

bool Shader::SetUniform(const std::string& name, const glm::mat4& matrix) {
	GLint uniform_value_loc = glGetUniformLocation(shader_id_, name.c_str());
	if (uniform_value_loc == -1) {
		LOG(ERROR) << "Could not find a variable!";
		return false;
	}
	return SetUniformExt(uniform_value_loc, matrix);
}

bool Shader::SetUniform(const std::string& name, float value) {
	GLint uniform_value_loc = glGetUniformLocation(shader_id_, name.c_str());
	if (uniform_value_loc == -1) {
		LOG(ERROR) << "Could not find a variable!";
		return false;
	}
	return SetUniformExt(uniform_value_loc, value);
}

bool Shader::SetUniform(const std::string& name, int value) {
	GLint uniform_value_loc = glGetUniformLocation(shader_id_, name.c_str());
	if (uniform_value_loc == -1) {
		LOG(ERROR) << "Could not find a variable!";
		return false;
	}
	SetUniformExt(uniform_value_loc, value);
	return true;
}

bool Shader::SetUniformFromMemory(unsigned int id, const glm::mat4& matrix) {
	auto local_uniform = uniform_memory_.find(id);
	if (local_uniform == uniform_memory_.end()) {
		return false;
	}
	SetUniformExt(local_uniform->second, matrix);
	return true;
}

bool Shader::SetUniformFromMemory(unsigned int id, float value) {
	auto local_uniform = uniform_memory_.find(id);
	if (local_uniform == uniform_memory_.end()) {
		return false;
	}
	SetUniformExt(local_uniform->second, value);
	return true;
}

bool Shader::SetUniformFromMemory(unsigned int id, int value) {
	auto local_uniform = uniform_memory_.find(id);
	if (local_uniform == uniform_memory_.end()) {
		return false;
	}
	SetUniformExt(local_uniform->second, value);
	return true;
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

GLint Shader::GetUniform(const std::string& name) const {
	GLint uniform_value_loc = glGetUniformLocation(shader_id_, name.c_str());
	if (uniform_value_loc == -1) {
		LOG(ERROR) << "Could not find a variable!";
	}
	return uniform_value_loc;
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
	LOG(INFO) << __FUNCTION__ << ", " << ToString();
	ForAllObservers([this](AssetObserver* observer){
		observer->OnDelete(*this);
	});
	Clear();
}

bool Shader::IsCompiled() const {
	return status_;
}

void Shader::SetStatus(bool status) {
	status_ = status;
}

bool Shader::AddShader(GLuint program, const std::string& shader_code, GLenum shader_type) {
	GLuint gl_shader = glCreateShader(shader_type);

	const GLchar* code[1] = { shader_code.c_str() };
	GLint code_lenght[1] = { static_cast<GLint>(shader_code.size()) };

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