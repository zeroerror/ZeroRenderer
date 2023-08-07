#include "EditorUICanvasNode.h"
#include <iostream>

EditorUICanvasNode::EditorUICanvasNode(const Rect& rect, const AlignType& alignType) {
	this->rect = rect;
	this->alignType = alignType;
}

EditorUICanvasNode::~EditorUICanvasNode() {
	if (father != nullptr) delete father;
	if (child != nullptr) delete child;
}
