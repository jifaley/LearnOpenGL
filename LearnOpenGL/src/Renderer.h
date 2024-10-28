#pragma once
#include <GL/glew.h>
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"



class Renderer
{
private:

public:
	Renderer();
	~Renderer();
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void DrawWithIndexCount(const VertexArray & va, const IndexBuffer & ib, const Shader & shader, int indexCount) const;
	void Clear() const;
};