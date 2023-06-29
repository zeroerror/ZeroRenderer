#pragma once
#include <string>
#include <Shader.h>
#include <unordered_map>

using namespace std;

class ShaderRepo {

public:
	ShaderRepo();
	~ShaderRepo();

	bool TryLoadShaderByGUID(const string& guid, Shader*& shader);
	bool TryLoadShaderByAssetPath(const string& path, Shader*& shader);

private:
	std::unordered_map<std::string, Shader*> allShaders_sortedByPath;
	std::unordered_map<std::string, Shader*> allShaders_sortedByGUID;
	bool _TryGetShaderByGUID(const std::string& guid, Shader*& shader);

};

