#include "tests/Camera3DCubeTest.h"
#include "tests/LightTest.h"
#include "GLDebug.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glm/gtx/quaternion.hpp>

int main() {
	GLFWwindow* window;
	const char* glsl_version = "#version 330 core";

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	int screen_width = 1280;
	int screen_height = 1080;
	window = glfwCreateWindow(screen_width, screen_height, "My OpenGL Window", NULL, NULL);
	if (!window) {
		GLCall(glfwTerminate());
		return -1;
	}

	// GLFW Context
	GLCall(glfwMakeContextCurrent(window));

	// Enable VSync
	GLCall(glfwSwapInterval(1));

	if (glewInit() != GLEW_OK)
		std::cout << "ERROR" << std::endl;



	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		// IMGUI Context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();

		// TEST
		test::Test* curTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(curTest);
		curTest = testMenu;

		testMenu->RegisterTest("Camera3D Cube Test", [window, screen_width, screen_height]() {
			test::Camera3DCubeTest* test = new test::Camera3DCubeTest();
			test->Ctor(window, screen_width, screen_height);
			return test;
			}
		);

		testMenu->RegisterTest("Light Test", [window, screen_width, screen_height]() {
			test::LightTest* test = new test::LightTest();
			test->Ctor(window, screen_width, screen_height);
			return test;
			}
		);

		//-------------------------------------------------------------------------
		unsigned int vao;
		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));

		unsigned int vbo;
		GLCall(glGenBuffers(1, &vbo));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
		float halfWidth = 0.1;
		float halfHeight = 0.1;
		float halfDepth = 0.1;
		float* data = new float[40]{
			// 顶点坐标 + 纹理坐标
			-halfWidth, -halfHeight, -halfDepth, 0.0f, 0.0f,     // 顶点0
			halfWidth, -halfHeight, -halfDepth, 1.0f, 0.0f,      // 顶点1
			halfWidth, halfHeight, -halfDepth, 1.0f, 1.0f,       // 顶点2
			-halfWidth, halfHeight, -halfDepth, 0.0f, 1.0f,      // 顶点3
			-halfWidth, -halfHeight, halfDepth, 0.0f, 0.0f,      // 顶点4
			halfWidth, -halfHeight, halfDepth, 1.0f, 0.0f,       // 顶点5
			halfWidth, halfHeight, halfDepth, 1.0f, 1.0f,        // 顶点6
			-halfWidth, halfHeight, halfDepth, 0.0f, 1.0f        // 顶点7
		};
		GLuint size = 40 * sizeof(GL_FLOAT);
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

		GLuint stride = 5 * sizeof(GL_FLOAT);
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (const void*)0));
		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (const void*)(3 * sizeof(GL_FLOAT))));

		GLuint ibo;
		GLCall(glGenBuffers(1, &ibo));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
		GLuint indiceArray[36] = {
	0, 1, 2,  // 面0
	2, 3, 0,
	1, 5, 6,  // 面1
	6, 2, 1,
	5, 4, 7,  // 面2
	7, 6, 5,
	4, 0, 3,  // 面3
	3, 7, 4,
	3, 2, 6,  // 面4
	6, 7, 3,
	4, 5, 1,  // 面5
	1, 0, 4
		};
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint), indiceArray, GL_STATIC_DRAW));

		Shader shader = Shader("res/shader/CubeTest.shader");
		shader.Bind();
		glm::quat rot = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS));
		//	GLCall(glDepthMask(GL_TRUE));
			//-------------------------------------------------------------------------

		while (!glfwWindowShouldClose(window)) {
			GLCall(glfwPollEvents());
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

			//--------------------------------------------------------------
			GLCall(glBindVertexArray(vao));
			float rotationSpeed = 0.01f;
			glm::quat rotationDelta = glm::angleAxis(rotationSpeed, glm::vec3(1.0f, 1.0f, 0.0f));
			rot = rot * rotationDelta;
			shader.SetUniformMat4f("u_ModRotationMatrix", glm::toMat4(rot));
			GLCall(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr));
			//--------------------------------------------------------------

			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			if (curTest) {
				curTest->OnUpdate(0.1f);
				curTest->OnRender();
				ImGui::Begin("Test");
				if (curTest != testMenu && ImGui::Button("<-")) {
					delete curTest;
					curTest = testMenu;
				}
				curTest->OnImGuiRender();
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			GLCall(glfwSwapBuffers(window));
		}

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
