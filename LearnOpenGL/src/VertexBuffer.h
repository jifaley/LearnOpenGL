#pragma once

class VertexBuffer
{
private:
	unsigned int m_RenderId;
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(const void* data, unsigned int size, bool isDynamic);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;
};