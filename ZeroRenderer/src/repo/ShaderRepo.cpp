#include "ShaderRepo.h"
#include "Database.h"
#include <iostream>

ShaderRepo::ShaderRepo() {
	allShaders_sortedByPath = std::unordered_map<std::string, Shader*>();
	allShaders_sortedByGUID = std::unordered_map<std::string, Shader*>();
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

void ShaderRepo::LoadShaderByPath(const string& path, Shader*& shader) {
	string guid = Database::GetGUIDFromAssetPath(path);
	if (guid == "") {
		std::cout << "############## ShaderRepo::LoadShaderByPath -  path doesn't exist : " << path << std::endl;
		return;
	}

	if (!TryGetShaderByGUID(guid, shader)) {
		shader = new Shader(path);
	}

	allShaders_sortedByGUID.insert(std::pair<std::string, Shader*>(guid, shader));
	allShaders_sortedByPath.insert(std::pair<std::string, Shader*>(path, shader));
	std::cout << "ShaderRepo::LoadShader. Path - " << path << " GUID - " << guid << std::endl;
	return;
}

void ShaderRepo::LoadShaderByGUID(const string& guid, Shader*& shader) {
	string path = Database::GetAssetPathFromGUID(guid);
	if (path == "") {
		std::cout << "############## ShaderRepo::LoadShaderByGUID -  guid doesn't exist : " << guid << std::endl;
		return;
	}

	if (!TryGetShaderByGUID(guid, shader)) {
		shader = new Shader(path);
	}

	allShaders_sortedByGUID.insert(std::pair<std::string, Shader*>(guid, shader));
	allShaders_sortedByPath.insert(std::pair<std::string, Shader*>(path, shader));
	std::cout << "ShaderRepo::LoadShader. Path - " << path << " GUID - " << guid << std::endl;
	return;
}

bool ShaderRepo::TryGetShaderByGUID(const std::string& guid, Shader*& shader) {
	std::unordered_map<std::string, Shader*>::iterator it = allShaders_sortedByGUID.find(guid);
	if (it != allShaders_sortedByGUID.end()) {
		shader = it->second;
		return true;
	}

	std::cout << "  ################ GetShaderByGUID: " << guid << " not found" << std::endl;
	return false;
}