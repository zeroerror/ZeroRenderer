#include "EditorUICanvasNode.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glm/glm.hpp>

#include <iostream>

using namespace glm;

EditorUICanvasNode::EditorUICanvasNode(const Rect &rect, const AlignType &alignType)
{
	this->rect = rect;
	this->alignType = alignType;
}

EditorUICanvasNode::~EditorUICanvasNode()
{
	if (father != nullptr)
		delete father;
	if (child != nullptr)
		delete child;
}

void EditorUICanvasNode::Draw()
{
	if (drawCb != nullptr)
	{
		glm::vec2 pos = this->rect.GetLeftTop();
		ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y));
		ImGui::SetNextWindowSize(ImVec2(this->rect.width, this->rect.height));
		drawCb(this->rect);
	}
	if (child != nullptr)
		child->Draw();
}

bool EditorUICanvasNode::IsInCanvas(const glm::vec2 &pos)
{
	vec2 leftTop = this->rect.GetLeftTop();
	vec2 rightBottom = this->rect.GetRightBottom();
	return pos.x >= leftTop.x && pos.x <= rightBottom.x && pos.y >= leftTop.y && pos.y <= rightBottom.y;
}
