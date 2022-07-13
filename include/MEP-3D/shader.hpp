#ifndef SHADER_HPP
#define SHADER_HPP

#include<string>
#include<fstream>
#include<GL/glew.h>
#include<glm/glm.hpp>
#include<MEP-3D/identity.hpp>

class ShaderBase: public Identity {
public:
	ShaderBase() : Identity(__FUNCTION__), status_(false) {}

	virtual bool Compile(const std::string&, const std::string&) = 0;
	bool IsCompiled() const;
	virtual GLuint GetProjectionLocation() = 0;
	virtual GLuint GetModelLocation() = 0;
	virtual GLuint GetViewLocation() = 0;
	virtual bool SetUniformExt(GLuint uniform_location, const glm::mat4& matrix) = 0;
	virtual bool SetUniformExt(GLuint uniform_location, float value) = 0;
	virtual bool SetUniformExt(GLuint uniform_location, int value) = 0;
	virtual bool SetUniform(const std::string& name, const glm::mat4& matrix) = 0;
	virtual bool SetUniform(const std::string& name, float value) = 0;
	virtual bool SetUniform(const std::string& name, int value) = 0;
	virtual ~ShaderBase() = default;

protected:
	void SetStatus(bool status);

private:
	bool status_;
};

class ShaderBinder {
public:
	void Bind(ShaderBase* shader);
	void UnBind();
protected:
	ShaderBase* Get();

private:
	ShaderBase* shader_;
};

class Shader : public ShaderBase {
public:
	Shader();
	bool Compile(const std::string& vertex_code, const std::string& fragment_code) override;
	void StartUsing();
	void StopUsing();
	bool CreateFromFile(const std::string& vertex_location, const std::string& fragment_location);
	bool CreateFromString(const std::string& vertex_code, const std::string& fragment_code);
	void Clear();
	GLuint GetProjectionLocation() override;
	GLuint GetModelLocation() override;
	GLuint GetViewLocation() override;
	bool SetUniformExt(GLuint uniform_location, const glm::mat4& matrix) override;
	bool SetUniformExt(GLuint uniform_location, float value) override;
	bool SetUniformExt(GLuint uniform_location, int value) override;
	bool SetUniform(const std::string& name, const glm::mat4& matrix) override;
	bool SetUniform(const std::string& name, float value) override;
	bool SetUniform(const std::string& name, int value) override;
	static std::string LoadFromFile(const std::string& path);
	virtual ~Shader();

protected:
	GLuint shader_id_;
	GLuint uniform_projection_;
	GLuint uniform_model_;
	GLuint uniform_view_;

private:
	bool AddShader(GLuint program, const std::string& shader_code, GLenum shader_type);
	bool GetUniformLocation(GLuint &location, const std::string &name);

};

#endif