#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <stdio.h>
#include <string.h>

#include "Shader.h"
#include "Renderer.h"
#include "GLDebug.h"

Shader::Shader(const std::string& filePath) {
	ShaderProgramSource source = ParseShader(filePath);
	m_programID = CreateShader(source.VertexSource, source.FragmentSource);
	GLCall(glUseProgram(m_programID));
	std::cout << "Shader::Shader() " << m_programID << std::endl;
}

Shader::~Shader() {
	std::cout << "Shader::~Shader() " << m_programID << std::endl;
	GLCall(glDeleteShader(m_vsID));
	GLCall(glDeleteShader(m_fsID));
	GLCall(glDeleteProgram(m_programID));
}

unsigned int Shader::GetID() const { return m_programID; }

ShaderProgramSource Shader::ParseShader(const std::string& filePath) {
	std::ifstream stream(filePath);
	if (!stream.is_open()) {
		ASSERT(false);
	}

	enum class ShaderType {
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else {
			ss[(int)type] << line << "\n";
		}
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	// Error handling
	int status;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &status));
	if (status == GL_FALSE) {
		int len;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len));
		char* msg = (char*)alloca(len * sizeof(char));
		GLCall(glGetShaderInfoLog(id, len, &len, msg));
		std::cout << "Compile Error=> " << (type == GL_VERTEX_SHADER ? "VertexShader" : "FragmentShder")
			<< std::endl;
		std::cout << msg << std::endl;
		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int programID = glCreateProgram();
	m_vsID = CompileShader(GL_VERTEX_SHADER, vertexShader);
	m_fsID = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(programID, m_vsID));
	GLCall(glAttachShader(programID, m_fsID));
	GLCall(glLinkProgram(programID));
	GLCall(glValidateProgram(programID));

	GLCall(glDeleteShader(m_vsID));
	GLCall(glDeleteShader(m_fsID));

	return programID;
}

void Shader::Bind() const {
	GLCall(glUseProgram(m_programID));
}

void Shader::UnBind() const {
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int v) {
	GLCall(glUniform1i(GetUniformLocation(name), v));
}

void Shader::SetUniform1f(const std::string& name, float v) {
	GLCall(glUniform1f(GetUniformLocation(name), v));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2) {
	GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

int Shader::GetUniformLocation(const std::string& name) {
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[name];
	}

	GLCall(int location = glGetUniformLocation(m_programID, name.c_str()));
	m_UniformLocationCache[name] = location;

	if (location == -1) {
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	}

	return location;
}

std::string Shader::ReadShaderFromFile(const char* filepath) {
	std::string fullpath;
	if (filepath[0] == '.' && filepath[1] == '/') {
		char* cwd = GetCurrentDir(1024);
		fullpath = std::string(cwd) + std::string("/") + std::string(filepath + 2);
		delete[] cwd;
	}
	else {
		fullpath = filepath;
	}

	std::ifstream stream(fullpath);
	if (!stream.is_open()) {
		std::cout << "Can't Not Open File: " << fullpath << std::endl;
		return "";
	}

	std::stringstream buffer;
	buffer << stream.rdbuf();

	return buffer.str();
}

char* Shader::GetCurrentDir(size_t size) {
	char* buf = new char[size];
#if defined(_WIN32)
	GetCurrentDirectoryA(size, buf);
#else
	getcwd(buf, size);
#endif
	return buf;
}