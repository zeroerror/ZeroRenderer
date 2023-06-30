#include "ShaderRepo.h"
#include "Database.h"
#include <iostream>

ShaderRepo::ShaderRepo() {
	allShaders_sortedByGUID = std::unordered_map<std::string, Shader*>();
	allShaders_sortedByPath = std::unordered_map<std::string, Shader*>();
}

ShaderRepo::~ShaderRepo() {
	std::cout << "ShaderRepo::~ShaderRepo() " << std::endl;
	for (auto& shader : allShaders_sortedByPath) {
		shader.second->~Shader();
	}
	for (auto& shader : allShaders_sortedByGUID) {
		shader.second->~Shader();
	}
}

bool ShaderRepo::TryAddShader(const std::string& guid, Shader*& shader) {
	if(!Database::GUIDExist(guid)) {
		return false;
	}

	allShaders_sortedByGUID.insert(std::pair<std::string, Shader*>(guid, shader));
	
	string path;
	if (Database::TryGetAssetPathFromGUID(guid, path)) {
		allShaders_sortedByPath.insert(std::pair<std::string, Shader*>(path, shader));
	}

	return true;
}

bool ShaderRepo::TryGetShaderByGUID(const std::string& guid, Shader*& shader) {
	std::unordered_map<std::string, Shader*>::iterator it = allShaders_sortedByGUID.find(guid);
	if (it == allShaders_sortedByGUID.end()) {
		return false;
	}

	shader = it->second;
	return true;
}