#include "EditorUICanvas.h"
#include <iostream>

EditorUICanvas::EditorUICanvas(const Rect& rect, const AlignType& alignType) {
	_root = new EditorUICanvasNode(rect, alignType);
}

EditorUICanvas::~EditorUICanvas() {
	delete _root;
}

void EditorUICanvas::Add(EditorUICanvasNode* node) {
	AlignType alignType = this->_root->alignType;
	float canvasWidth = this->_root->rect.width;
	float canvasHeight = this->_root->rect.height;
	vec2 canvasLT = this->_root->rect.GetLeftTop();
	vec2 canvasRT = this->_root->rect.GetRightTop();

	vec2 curNodePos;
	if (alignType == AlignType::LeftTop) {
		curNodePos = canvasLT;
		EditorUICanvasNode* cur = this->_root;
		while (cur->child != nullptr) {
			cur = cur->child;
			cur->rect.SetXY_AsLeftTop(curNodePos);
			curNodePos.x += cur->rect.width;
			if (curNodePos.x >= canvasRT.x) {
				curNodePos.y += cur->rect.height;
				curNodePos.x = 0;
			}
		}

		node->rect.SetXY_AsLeftTop(curNodePos);
		cur->child = node;

		return;
	}

	cout << "EditorUICanvas::Add(EditorUICanvasNode* node) Not Handle AlignType!" << endl;
}

void EditorUICanvas::Remove(EditorUICanvasNode* node) {
}

void EditorUICanvas::Draw() {
	_root->Draw();
}
