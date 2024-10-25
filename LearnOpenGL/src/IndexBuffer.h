#pragma once

class IndexBuffer
{
private:
	unsigned int m_RenderId;
	unsigned int m_Count; //具体有多少个索引
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;

	inline unsigned int GetCount() const { return m_Count; }
};