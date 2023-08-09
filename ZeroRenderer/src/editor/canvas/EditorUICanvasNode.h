#pragma once 
#include "Rect.h"
#include "AlignType.h"
#include <vector>
#include <functional>

using namespace std;

class EditorUICanvasNode {

public:
	EditorUICanvasNode(const Rect& rect, const AlignType& alignType);
	~EditorUICanvasNode();

	Rect rect;
	AlignType alignType;

	typedef function<void(const Rect& rect)> Callback;
	Callback callback;
    void Draw();

public:
	EditorUICanvasNode* father;
	EditorUICanvasNode* child;

};