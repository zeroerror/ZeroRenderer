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
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "GLDebug.h"

Shader::Shader(const string& filePath) {
	ShaderProgramSource source = ParseShader(filePath);
	m_programID = CreateShader(source.VertexSource, source.FragmentSource);
	GLCall(glUseProgram(m_programID));
	_path = filePath;
	cout << "Shader::Shader() " << m_programID << endl;
}

Shader::~Shader() {
	cout << "Shader::~Shader() " << m_programID << endl;
	GLCall(glDeleteShader(m_vsID));
	GLCall(glDeleteShader(m_fsID));
	GLCall(glDeleteProgram(m_programID));
}

unsigned int Shader::GetID() const { return m_programID; }
string Shader::GetPath() const { return _path; }

ShaderProgramSource Shader::ParseShader(const string& filePath) {
	ifstream stream(filePath);
	if (!stream.is_open()) {
		ASSERT(false);
	}

	string line;
	stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != string::npos) {
			if (line.find("vertex") != string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != string::npos)
				type = ShaderType::FRAGMENT;
		}
		else {
			ss[(int)type] << line << "\n";
		}
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const string& source) {
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
		cout << "Compile Error=> " << (type == GL_VERTEX_SHADER ? "VertexShader" : "FragmentShder")
			<< endl;
		cout << msg << endl;
		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const string& vertexShader, const string& fragmentShader) {
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

void Shader::SetUniform1i(const string& name, int v) {
	GLCall(glUniform1i(GetUniformLocation(name), v));
}

void Shader::SetUniform1f(const string& name, float v) {
	GLCall(glUniform1f(GetUniformLocation(name), v));
}

void Shader::SetUniform4f(const string& name, float v0, float v1, float v2, float v3) {
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform3f(const string& name, float v0, float v1, float v2) {
	GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniformMat4f(const string& name, const glm::mat4& matrix) {
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

int Shader::GetUniformLocation(const string& name) {
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[name];
	}

	GLCall(int location = glGetUniformLocation(m_programID, name.c_str()));
	m_UniformLocationCache[name] = location;

	if (location == -1) {
		cout << "Warning: uniform '" << name << "' doesn't exist!" << endl;
	}

	return location;
}

string Shader::ReadShaderFromFile(const char* filepath) {
	string fullpath;
	if (filepath[0] == '.' && filepath[1] == '/') {
		char* cwd = GetCurrentDir(1024);
		fullpath = string(cwd) + string("/") + string(filepath + 2);
		delete[] cwd;
	}
	else {
		fullpath = filepath;
	}

	ifstream stream(fullpath);
	if (!stream.is_open()) {
		cout << "Can't Not Open File: " << fullpath << endl;
		return "";
	}

	stringstream buffer;
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