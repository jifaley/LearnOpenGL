#include "TestBatchRender.h"
#include "utils.h"
#include "imgui/imgui.h"



#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
	TestBatchRender::TestBatchRender()
		:m_Translation(200, 200, 0)
		
	{
		float positions[] =
		{
			-100.0f,  -100.0f,  0.0f, 0.0f,//0
			-50.0f,   -100.0f,  1.0f, 0.0f,//1
			-50.0f,   -50.0f,   1.0f, 1.0f,//2
			-100.0f,  -50.0f,   0.0f, 1.0f,//3

			 50.0f,   50.0f,   0.0f, 0.0f,//4
			 100.0f,  50.0f,   1.0f, 0.0f,//5
			 100.0f,  100.0f,  1.0f, 1.0f,//6
			 50.0f,   100.0f,  0.0f, 1.0f //7


		};
		//一行四个数，前两个位置坐标，后两个纹理坐标



		/* 索引缓冲区所需索引数组 */
		unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_VAO = std::make_unique<VertexArray>();

		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 8 * 4 * sizeof(float)); //4个顶点 * 每个顶点4个float

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);

		m_Proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		//投影后得到了更小的logo
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//值是负的，因为移动相机实际上相当于反向移动物体。在这里我们只能移动物体

		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

		m_Texture = std::make_unique<Texture>("res/textures/ChernoLogo.png");
		m_Shader->SetUniform1i("u_Texture", 0); //纹理插槽0
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

		Renderer renderer;

		m_Texture->Bind();

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader); //va,ib在Draw()函数中自带了Bind()，因此不在这里再显式绑定了
		}

	}
	void TestBatchRender::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", &m_Translation.x, 0.0f, 960.0f); //这里的float3提供第一个float地址后会自动绑定后面的两个float
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}