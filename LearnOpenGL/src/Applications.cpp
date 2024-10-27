#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h";
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"




int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	


	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1); //����ˢ��Ƶ��




	//ע��:glewInit()�����ڴ�������Ч��opengl�����ĺ�ſ���ʹ��!
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	//������Ҫһ����ʱ������������vb,ib���뿪֮ǰ��������������Ῠס
	//��סԭ������������������Ҫ�����ģ�����context��main()�˳�ǰ�͹ر��ˣ�����������ʱ��û��context��ʧЧ
	//���һ��������Ϳ�����vb,ib��ǰ������
	{

		float positions[] =
		{
			100.0f, 100.0f,  0.0f, 0.0f, //0
			200.0f, 100.0f,  1.0f, 0.0f,//1
			200.0f, 200.0f,  1.0f, 1.0f,//2
			100.0f, 200.0f,  0.0f, 1.0f//3
		};
		//һ���ĸ�����ǰ����λ�����꣬��������������



		/* ���������������������� */
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float)); //4������ * ÿ������4��float
		
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb,layout);

		

		IndexBuffer ib(indices, 6);

		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		//ͶӰ��õ��˸�С��logo
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
		//ֵ�Ǹ��ģ���Ϊ�ƶ����ʵ�����൱�ڷ����ƶ����塣����������ֻ���ƶ�����

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));

		glm::mat4 mvp = proj * view * model;



		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
		shader.SetUniformMat4f("u_MVP", mvp);

		Texture texture("res/textures/ChernoLogo.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0); //������0



		va.UnBind();
		shader.UnBind();
		vb.UnBind();
		ib.UnBind();
		

		Renderer renderer;

	


		float r = 0.0f;
		float increment = 0.05f;


		/* Loop until the user closes the window */ //��ѭ��
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			va.Bind();
			ib.Bind();

			renderer.Draw(va, ib, shader);


			//ע�⣡��Щint����Ҫ���޷������ͣ�
			if (r > 1.0f)
				increment = -0.05f;
			if (r < 0.0f)
				increment = 0.05f;
			r += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}


	glfwTerminate();
	return 0;
}


