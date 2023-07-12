#include "MeshFilter.h"

MeshFilter::MeshFilter() {
	vertices = new std::vector<Vertex*>();
	indices = new std::vector<unsigned int>();
}
