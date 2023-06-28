#pragma once
#include <string>
#include <glm/ext/vector_float4.hpp>
#include <iostream>
#include <fstream>

#include "Database.h"

class MaterialMetadata {

public:
	MaterialMetadata(){};
	~MaterialMetadata(){};

	std::string guid;

	void SerializeTo(const std::string& path);
	void DeserializeFrom(const std::string& path);

};

