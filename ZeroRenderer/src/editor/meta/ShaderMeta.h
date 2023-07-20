#pragma once
#include <glm/ext/vector_float4.hpp>
#include <string>
#include <vector>
#include <any>
#include "ShaderUniformType.h";

using namespace std;

struct ShaderUniform {
	string name;
	ShaderUniformType_ type;
	std::any value;
};

class ShaderMeta {

public:
	ShaderMeta();
	~ShaderMeta();

	string guid;

	vector<ShaderUniform> uniforms;

};

