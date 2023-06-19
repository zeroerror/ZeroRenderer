#include "Vertex.h"

void Vertex::SetPosition(const float& x, const float& y, const float& z) {
	this->position = glm::vec3(x, y, z);
}

void Vertex::SetNormal(const float& x, const float& y, const float& z) {
	this->normal = glm::vec3(x, y, z);
}

void Vertex::SetTexCoords(const float& x, const float& y) {
	this->texCoords = glm::vec2(x, y);
}