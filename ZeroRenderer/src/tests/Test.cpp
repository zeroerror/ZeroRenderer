#include "Test.h"

namespace test {

	TestMenu::TestMenu(std::shared_ptr<Test>& currentTestPointer)
		:m_CurrentTest(currentTestPointer) {

	}

	TestMenu::~TestMenu() {

	}

	void TestMenu::OnImGuiRender() {
		for (auto& test : m_Tests) {
			if (ImGui::Button(test.first.c_str())) {
				m_CurrentTest = test.second();
			}
		}
	}

}