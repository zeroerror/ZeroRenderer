#include "Renderer.h"

#include <string>
#include <iostream>

#include "GLDebug.h"

void Renderer::Clear() const {
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray* va, const IndexBuffer* ib, const Shader* shader, const Texture* texture) const {
	va->Bind();
	ib->Bind();
	shader->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
}

