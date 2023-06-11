#pragma once
#include <unordered_map>
#include <Shader.h>

class ShaderRepo {

public:
	ShaderRepo();
	~ShaderRepo();

	unsigned int LoadShader(const char* path);
	Shader* GetShader(const unsigned int& shaderID);

private:
	std::unordered_map<unsigned int, Shader*> allShaders;

};

