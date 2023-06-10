#include "ShaderManager.h"

ShaderManager::ShaderManager() {
    allShaders = std::unordered_map<unsigned int, Shader*>();
}

ShaderManager::~ShaderManager() {}

unsigned int ShaderManager::LoadShader(const char* path) {
	Shader* shader = new Shader(path);
	unsigned int shaderID = shader->GetID();
	allShaders.insert(std::pair<unsigned int, Shader*>(shaderID, shader));
	return shaderID;
}

void ShaderManager::UseShader(unsigned int shader) {
	allShaders[shader]->Bind();
}

void ShaderManager::DeleteShader(unsigned int shader) {
	allShaders[shader]->~Shader();
}

void ShaderManager::DeleteAllShader() {
	for (int i = 0; i < allShaders.size(); i++) {
		allShaders[i]->~Shader();
	}
}