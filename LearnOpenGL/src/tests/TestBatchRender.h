#pragma once
#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Renderer.h"

#include <memory>

namespace test
{
	class TestBatchRender : public Test
	{
	public:
		TestBatchRender();
		~TestBatchRender();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture1;
		std::unique_ptr<Texture> m_Texture2;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;

		float m_QuadPosition1[2]{ -50.0f, -50.0f };
		float m_QuadPosition2[2]{ 50.0f, 50.0f };
		glm::vec3 m_Translation;

		glm::mat4 m_Proj, m_View;
	};
}