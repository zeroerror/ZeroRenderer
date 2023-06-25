#pragma once
#include <unordered_map>
#include <Shader.h>

class ShaderRepo {

public:
	ShaderRepo();
	~ShaderRepo();

	std::string LoadShader(const char* path);
	Shader* GetShaderByGUID(const std::string& guid);

private:
	std::unordered_map<std::string, Shader*> allShaders;

};

