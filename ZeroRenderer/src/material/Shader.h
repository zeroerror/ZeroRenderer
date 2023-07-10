#pragma once

#include <string>
#include <unordered_map>
#include <glm/ext/matrix_float4x4.hpp>

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {

private:
	unsigned int m_programID;
	unsigned int m_vsID;
	unsigned int m_fsID;
	std::string path;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	bool useLightingMVP;

	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void UnBind()const;

	unsigned int GetID() const;

	// Set Uniforms
	void SetUniform1i(const std::string& name, int v);
	void SetUniform1f(const std::string& name, float v);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	int GetUniformLocation(const std::string& name);

private:
	ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	std::string ReadShaderFromFile(const char* filepath);
	char* GetCurrentDir(size_t count);

};
