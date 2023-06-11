#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"

class Renderer {

public:
	void Clear() const;
	void Draw(const VertexArray* va, const IndexBuffer* ib, const Shader* shader,const Texture* texture) const;
};