#include "Mesh.h"

Mesh::Mesh() {
	vertices = new std::vector<Vertex*>();
	indices = new std::vector<unsigned int>();
}
