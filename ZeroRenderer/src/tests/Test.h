#pragma once 

#include <GL/glew.h>
#include <ImGui/imgui.h>
#include <functional>
#include <vector>

#include "Renderer.h"

namespace test {

	class Test {

	public:
		Test() {}
		virtual	~Test() {}

		virtual void OnUpdate(const float& deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

	};

	class TestMenu:public Test {
	public:
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