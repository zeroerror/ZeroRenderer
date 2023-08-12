#pragma once
#include <string>
#include <Shader.h>
#include <unordered_map>

using namespace std;

class ShaderRepo {

public:
	ShaderRepo();
	~ShaderRepo();

	bool TryAddShader(const string& guid, Shader*& shader);
	bool TryGetShaderByGUID(const std::string& guid, Shader*& shader);

	Shader* ErrorShader();
	Shader* DefaultLightShader();

private:
	std::unordered_map<std::string, Shader*> allShaders_sortedByPath;
	std::unordered_map<std::string, Shader*> allShaders_sortedByGUID;

	Shader* _errorShader;
	void _CreateErrorShader();

	Shader *_defaultLightShader;
	void _CreateDefaultLightShader();
};

