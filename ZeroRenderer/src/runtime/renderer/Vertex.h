#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using namespace glm;

class Vertex {

public:
	Vertex();
	Vertex(const vec3& pos, const vec3& nor, const vec2& tex);
	~Vertex();

	void SetPosition(const float& x, const float& y, const float& z);
	void SetNormal(const float& x, const float& y, const float& z);
	void SetTexCoords(const float& x, const float& y);

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;

};
