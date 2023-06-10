#include "Renderer.h"
#include "GLDebug.h"
#include <GL/glew.h>
#include <string>

#include <iostream>

void Renderer::Clear() const {
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray* va, const IndexBuffer* ib, const Material* material) const {
	va->Bind();
	ib->Bind();
	material->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
}

