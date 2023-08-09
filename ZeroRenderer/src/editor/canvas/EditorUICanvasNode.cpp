#include "EditorUICanvasNode.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glm/glm.hpp>

#include <iostream>

EditorUICanvasNode::EditorUICanvasNode(const Rect& rect, const AlignType& alignType) {
	this->rect = rect;
	this->alignType = alignType;
}

EditorUICanvasNode::~EditorUICanvasNode() {
	if (father != nullptr) delete father;
	if (child != nullptr) delete child;
}

void EditorUICanvasNode::Draw() {
	if(callback != nullptr){
		glm::vec2 pos = this->rect.GetLeftTop();
		ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y));
		ImGui::SetNextWindowSize(ImVec2(this->rect.width, this->rect.height));	
		callback(this->rect);
	}

	if (child != nullptr) child->Draw();
}
