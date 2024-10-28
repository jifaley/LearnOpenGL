#include "TestBatchRender.h"
#include "utils.h"
#include "imgui/imgui.h"



#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <array>
#include <iostream>

namespace test
{

	struct Vec3
	{
		float x, y, z;
	};

	struct Vec2
	{
		float x, y;
	};

	struct Vec4
	{
		float r, g, b, a;
	};
	struct Vertex
	{
		Vec2 Position;
		float TexId;
		Vec2 TexCoords;
		Vec4 Color;
	};

	static Vertex* CreateQuad(Vertex* target, float x, float y, float textureID)
	{
		float size = 50.0f;


		target->Position = { x, y};
		target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
		target->TexCoords = { 0.0f, 0.0f };
		target->TexId = textureID;
		target++;


		target->Position = { x + size, y };
		target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
		target->TexCoords = { 1.0f, 0.0f };
		target->TexId = textureID;
		target++;
		  
		target->Position = { x + size, y + size };
		target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
		target->TexCoords = { 1.0f, 1.0f };
		target->TexId = textureID;
		target++;

		target->Position = { x, y + size };
		target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
		target->TexCoords = { 0.0f, 1.0f };
		target->TexId = textureID;
		target++;

		return target;
	}
	const size_t MaxQuadCount = 1000;
	const size_t MaxVertexCount = MaxQuadCount * 4;
	const size_t MaxIndexCount = MaxQuadCount * 6;

	TestBatchRender::TestBatchRender()
		:m_Translation(200, 200, 0)
		
	{
		
		

		/* 索引缓冲区所需索引数组 */
	/*	unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};*/
		unsigned int indices[MaxIndexCount];
		unsigned int offset = 0;
		for (int i = 0; i < MaxIndexCount; i += 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;
			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;
			offset += 4;
		}


		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_VAO = std::make_unique<VertexArray>();

		m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, MaxVertexCount * sizeof(Vertex), true); //8个顶点 * 每个顶点8个float

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(1);
		layout.Push<float>(2);
		layout.Push<float>(4);
		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, MaxIndexCount);

		m_Proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		//投影后得到了更小的logo
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//值是负的，因为移动相机实际上相当于反向移动物体。在这里我们只能移动物体

		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

		m_Texture1 = std::make_unique<Texture>("res/textures/ChernoLogo.png");
		m_Texture2 = std::make_unique<Texture>("res/textures/HazelLogo.png");

		std::vector<int> textures{ 0,1 };
		m_Shader->SetUniform1iv("u_Textures", textures);
		//Use SetUniform1iv ! Not SetUniform2i!

	}
	TestBatchRender::~TestBatchRender()
	{
	}
	void TestBatchRender::OnUpdate(float deltaTime)
	{
	}
	void TestBatchRender::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		unsigned int indexCount = 0;

		std::array<Vertex, 1000> vertices;
		Vertex* buffer = vertices.data();
		for (int y = 0; y < 5; y++)
		{
			for (int x = 0; x < 5; x++)
			{
				buffer = CreateQuad(buffer, x * 50, y * 50, (x + y) % 2);
				indexCount += 6;
			}
		}

		buffer = CreateQuad(buffer, m_QuadPosition1[0], m_QuadPosition1[1], 0.0f);
		indexCount += 6;

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices.data());

		Renderer renderer;

		m_Texture1->Bind(0);
		m_Texture2->Bind(1);

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			//renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader); //va,ib在Draw()函数中自带了Bind()，因此不在这里再显式绑定了
			renderer.DrawWithIndexCount(*m_VAO, *m_IndexBuffer, *m_Shader, indexCount);
			//调用新的Draw()函数，具体渲染数字与传入数字有关
		}

	}
	void TestBatchRender::OnImGuiRender()
	{
		ImGui::DragFloat2("Quad Position1", m_QuadPosition1, 1.0f);
		ImGui::DragFloat2("Quad Position2", m_QuadPosition2, 1.0f);

		ImGui::SliderFloat3("Translation A", &m_Translation.x, 0.0f, 960.0f); //这里的float3提供第一个float地址后会自动绑定后面的两个float
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}