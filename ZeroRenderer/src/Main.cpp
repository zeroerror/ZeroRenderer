#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glm/gtx/quaternion.hpp>

#include "GLDebug.h"
#include "AssimpTest.h"
#include "PipelineTest.h"
#include "FileHelper.h"
#include "MaterialMetadata.h"

int main() {
	const char* glsl_version = "#version 330 core";
	GLFWwindow* window;
	ImGuiContext* imguiContext;

	/* Initialize the library */
	if (!glfwInit()) {
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	int screen_width = 840;
	int screen_height = 680;
	window = glfwCreateWindow(screen_width, screen_height, "Main Window", NULL, NULL);
	if (!window) {
		GLCall(glfwTerminate());
		return -1;
	}

	GLCall(glfwMakeContextCurrent(window));
	GLCall(glfwSwapInterval(1));

	if (glewInit() != GLEW_OK)
		std::cout << "ERROR" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	IMGUI_CHECKVERSION();
	imguiContext = ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	std::shared_ptr<test::Test> curTest = nullptr;
	test::TestMenu* testMenu = new test::TestMenu(curTest);
	testMenu->window = window;
	testMenu->imguiContext = imguiContext;
	testMenu->RegisterTest("Pipeline Test", []() {
		std::shared_ptr<test::PipelineTest> test = std::make_shared<test::PipelineTest>();
		test->Init();
		return test;
		});
	testMenu->RegisterTest("Assimp Test", []() {
		std::shared_ptr<test::AssimpTest> test = std::make_shared<test::AssimpTest>();
		test->Init();
		return test;
		});

	while (!glfwWindowShouldClose(window)) {
		GLCall(glfwMakeContextCurrent(window));
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		ImGui::SetCurrentContext(imguiContext);
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Test Menu");
		if (curTest != nullptr && ImGui::Button("<-")) {
			curTest = nullptr;
		}

		testMenu->OnImGuiRender();

		ImGui::SetCurrentContext(imguiContext);
		GLCall(glfwMakeContextCurrent(window));
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		GLCall(glfwPollEvents());
		GLCall(glfwSwapBuffers(window));

		if (curTest != nullptr) {
			curTest->OnUpdate(0.1f);
			curTest->OnRender();
			curTest->OnImGuiRender();
			if (curTest->isClosed) {
				curTest = nullptr;
			}
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
