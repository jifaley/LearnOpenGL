#include "VertexBuffer.h"
#include "utils.h"

VertexBuffer::VertexBuffer(const void * data, unsigned int size)
{
	//glGenBuffers(1, &buffer); //1ºÅ¾ä±ú
	//glBindBuffer(GL_ARRAY_BUFFER, buffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 2, positions, GL_STATIC_DRAW);


	GLCall(glGenBuffers(1, &m_RenderId)); //1ºÅ¾ä±ú
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderId));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RenderId));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderId));
}

void VertexBuffer::UnBind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
