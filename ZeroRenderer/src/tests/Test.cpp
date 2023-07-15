#include "Test.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "EditorDatabase.h"

namespace test {

	TestMenu::TestMenu(std::shared_ptr<Test>& currentTestPointer)
		:m_CurrentTest(currentTestPointer) {

	}

	TestMenu::~TestMenu() {

	}

	void TestMenu::OnImGuiRender() {
		if (ImGui::Button("Clear EditorDatabase")) {
			EditorDatabase::ClearMetaFile();
		}

		for (auto& test : m_Tests) {
			if (ImGui::Button(test.first.c_str())) {
				m_CurrentTest = test.second();
				glfwMakeContextCurrent(window);
				ImGui::SetCurrentContext(imguiContext);
			}
		}
	}

}