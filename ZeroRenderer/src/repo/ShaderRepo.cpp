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

bool ShaderRepo::TryLoadShaderByAssetPath(const string& path, Shader*& shader) {
	string guid;
	if (!Database::TryGetGUIDFromAssetPath(path, guid)) {
		std::cout << "############## ShaderRepo::TryLoadShaderByPath -  path doesn't exist : " << path << std::endl;
		return false;
	}

	shader = new Shader(path);
	allShaders_sortedByGUID.insert(std::pair<string, Shader*>(guid, shader));
	allShaders_sortedByPath.insert(std::pair<string, Shader*>(path, shader));
	std::cout << "ShaderRepo::LoadShader. Path - " << path << " GUID - " << guid << std::endl;
	return true;
}

bool ShaderRepo::TryLoadShaderByGUID(const string& guid, Shader*& shader) {
	string path;
	if (!Database::TryGetAssetPathFromGUID(guid, path)) {
		std::cout << "############## ShaderRepo::LoadShaderByGUID -  guid doesn't exist : " << guid << std::endl;
		return false;
	}

	if (_TryGetShaderByGUID(guid, shader)) {
		return true;
	}

	shader = new Shader(path);
	allShaders_sortedByGUID.insert(std::pair<std::string, Shader*>(guid, shader));
	allShaders_sortedByPath.insert(std::pair<std::string, Shader*>(path, shader));
	std::cout << "ShaderRepo::LoadShader. Path - " << path << " GUID - " << guid << std::endl;
	return true;
}

bool ShaderRepo::_TryGetShaderByGUID(const std::string& guid, Shader*& shader) {
	std::unordered_map<std::string, Shader*>::iterator it = allShaders_sortedByGUID.find(guid);
	if (it == allShaders_sortedByGUID.end()) {
		return false;
	}

	shader = it->second;
	return true;
}