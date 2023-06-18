#pragma once 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>
#include <functional>
#include <vector>

#include "Renderer.h"

namespace test {

	class Test {

	public:
		Test() { isClosed = false; }
		virtual	~Test() {}

		virtual void OnUpdate(const float& deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
		bool isClosed;

	};

	class TestMenu:public Test {
	public:
		GLFWwindow* window;
		ImGuiContext* imguiContext;

		TestMenu(std::shared_ptr<Test>& currentTestPointer);
		~TestMenu();

		void OnImGuiRender() override;

		void RegisterTest(const std::string& name, std::function<std::shared_ptr<Test>()> ctor) {
			m_Tests.push_back(std::make_pair(name, ctor));
		}


	private:
		std::shared_ptr<Test>& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<std::shared_ptr<Test> ()>>> m_Tests;
	};

}