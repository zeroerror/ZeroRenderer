#include "Mesh.h"

Mesh::Mesh() {
	meshFilter = new MeshFilter();
	meshRenderer= new MeshRenderer();
}

void Mesh::GenerateRenderer() {
	meshRenderer->GenerateRenderer(meshFilter);
}
