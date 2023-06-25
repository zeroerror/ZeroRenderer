#include "ShaderRepo.h"
#include <iostream>

#include "Database.h"

ShaderRepo::ShaderRepo() {
	allShaders = std::unordered_map<std::string, Shader*>();
}

ShaderRepo::~ShaderRepo() {
	std::cout << "ShaderRepo::~ShaderRepo() " << std::endl;
	for (auto& shader : allShaders) {
		shader.second->~Shader();
	}
}

std::string ShaderRepo::LoadShader(const char* path) {
	Shader* shader = new Shader(path);
	std::string guid = Database::GetGUIDFromAssetPath(path);
	allShaders.insert(std::pair<std::string, Shader*>(guid, shader));
	std::cout << "ShaderRepo::LoadShader **** path - " << path << " GUID - " << guid << std::endl;
	return guid;
}

Shader* ShaderRepo::GetShaderByGUID(const std::string& guid) {
	std::unordered_map<std::string, Shader*>::iterator it = allShaders.find(guid);
	if (it != allShaders.end()) {
		return allShaders.at(guid);
	}

	std::cout << " ################ GetShaderByGUID: " << guid << " not found" << std::endl;
	return nullptr;
}