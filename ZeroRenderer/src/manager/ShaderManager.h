#pragma once
#include <unordered_map>
#include <Shader.h>

class ShaderManager {

public:
	ShaderManager();
	~ShaderManager();

	unsigned int LoadShader(const char* path);
	void UseShader(unsigned int shader);
	void DeleteShader(unsigned int shader);
	void DeleteAllShader();

private:
	std::unordered_map<unsigned int, Shader*> allShaders;

};

