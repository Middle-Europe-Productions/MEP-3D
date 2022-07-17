#ifndef SHADER_HPP
#define SHADER_HPP

#include<string>
#include<fstream>
#include<GL/glew.h>
#include<glm/glm.hpp>
#include<MEP-3D/identity.hpp>
#include<MEP-3D/asset_observer.hpp>
#include<MEP-3D/observer_list.hpp>

class Shader : public Identity, public ObserverList<AssetObserver> {
public:
	Shader();
	bool Compile(const std::string& vertex_code, const std::string& fragment_code);
	bool IsCompiled() const;
	void StartUsing();
	void StopUsing();
	bool CreateFromFile(const std::string& vertex_location, const std::string& fragment_location);
	bool CreateFromString(const std::string& vertex_code, const std::string& fragment_code);
	void Clear();
	bool SetUniform(const std::string& name, const glm::mat4& matrix);
	bool SetUniform(const std::string& name, float value);
	bool SetUniform(const std::string& name, int value);
	bool SetUniformFromMemory(unsigned int id, const glm::mat4& matrix);
	bool SetUniformFromMemory(unsigned int id, float value);
	bool SetUniformFromMemory(unsigned int id, int value);
	bool SetUniformExt(GLuint uniform_location, const glm::mat4& matrix);
	bool SetUniformExt(GLuint uniform_location, float value);
	bool SetUniformExt(GLuint uniform_location, int value);
	bool SaveUniformToMemory(const std::string& name, unsigned int memory_id);
	GLint GetUniform(const std::string& name) const;
	static std::string LoadFromFile(const std::string& path);
	virtual ~Shader();

protected:
	std::unordered_map<unsigned int, GLuint> uniform_memory_;
	GLuint shader_id_;

private:
	void SetStatus(bool status);
	bool AddShader(GLuint program, const std::string& shader_code, GLenum shader_type);
	bool GetUniformLocation(GLuint &location, const std::string &name);
	bool status_;

};

#endif