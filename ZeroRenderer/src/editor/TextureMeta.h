#pragma once
#include <glm/ext/vector_float4.hpp>
#include <string>

class TextureMeta {

public:
	TextureMeta(){};
	~TextureMeta(){};

	std::string guid;
	unsigned int width;
	unsigned int height;

};

