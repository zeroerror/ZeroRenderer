#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Material.h"

class Renderer {

public:
	void Clear() const;
	void Draw(const VertexArray* va, const IndexBuffer* ib, const Material* material) const;
};