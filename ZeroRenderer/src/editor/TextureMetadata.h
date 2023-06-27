#pragma once
#include <glm/ext/vector_float4.hpp>
#include <string>

class TextureMetadata {

public:
	TextureMetadata(){};
	~TextureMetadata(){};

	std::string guid;
	unsigned int width;
	unsigned int height;

	void SerializeTo(const std::string& path);
	void DeserializeFrom(const std::string& path);

};

