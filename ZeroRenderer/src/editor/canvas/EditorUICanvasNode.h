#pragma once 
#include "Rect.h"
#include "AlignType.h"

#include <vector>
using namespace std;

class EditorUICanvasNode {

public:
	EditorUICanvasNode(const Rect& rect, const AlignType& alignType);
	~EditorUICanvasNode();

	Rect rect;
	AlignType alignType;

public:
	EditorUICanvasNode* father;
	EditorUICanvasNode* child;

};
