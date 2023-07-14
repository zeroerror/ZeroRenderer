#include "Mesh.h"

Mesh::Mesh() {
    vertices = new vector<Vertex*>();
    indices = new vector<unsigned int>();
}

Mesh::~Mesh() {
    for (int i = 0; i < vertices->size(); i++) {
        delete vertices->at(i);
    }
    delete vertices;
    delete indices;
}