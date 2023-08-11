#pragma once

#include <string>
#include <unordered_map>
#include <glm/ext/matrix_float4x4.hpp>

using namespace std;

struct ShaderProgramSource {
	string VertexSource;
	string FragmentSource;
};

enum class ShaderType {
	NONE = -1,
	VERTEX = 0,
	FRAGMENT = 1
};

class Shader {

public:
	Shader(const string& filePath);
	~Shader();

	void Bind() const;
	void UnBind()const;

	unsigned int GetID() const;
	string GetPath()const;

	// Set Uniforms
	void SetUniform1i(const string& name, int v);
	void SetUniform1f(const string& name, float v);
	void SetUniform3f(const string& name, float v0, float v1, float v2);
	void SetUniform4f(const string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const string& name, const glm::mat4& matrix);
	int GetUniformLocation(const string& name);


private:
	unsigned int m_programID;
	unsigned int m_vsID;
	unsigned int m_fsID;
	string _path;
	unordered_map<string, int> m_UniformLocationCache;

	ShaderProgramSource ParseShader(const string& filePath);
	unsigned int CompileShader(unsigned int type, const string& source);
	unsigned int CreateShader(const string& vertexShader, const string& fragmentShader);

	string ReadShaderFromFile(const char* filepath);
	char* GetCurrentDir(size_t count);

};
