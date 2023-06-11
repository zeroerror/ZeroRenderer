#include "ShaderRepo.h"
#include <iostream>

ShaderRepo::ShaderRepo() {
    allShaders = std::unordered_map<unsigned int, Shader*>();
}

ShaderRepo::~ShaderRepo() {
	for (auto& shader : allShaders) {
		shader.second->~Shader();
	}
}

unsigned int ShaderRepo::LoadShader(const char* path) {
	Shader* shader = new Shader(path);
	unsigned int shaderID = shader->GetID();
	allShaders.insert(std::pair<unsigned int, Shader*>(shaderID, shader));
	return shaderID;
}

Shader* ShaderRepo::GetShader(const unsigned int& shaderID) {
	return allShaders[shaderID];
}