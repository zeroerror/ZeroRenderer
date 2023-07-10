#pragma once
#include <glm/ext/vector_float4.hpp>
#include <string>

class ShaderMeta {

public:
	ShaderMeta(){};
	~ShaderMeta(){};

	std::string guid;
    bool useLightingMVP;

};

