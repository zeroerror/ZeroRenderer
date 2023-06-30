#pragma once
#include <glm/ext/vector_float4.hpp>
#include <string>

class ShaderMetadata {

public:
	ShaderMetadata(){};
	~ShaderMetadata(){};

	std::string guid;
    bool useLightingMVP;

	void SerializeTo(const std::string& path);
	void DeserializeFrom(const std::string& path);

};

