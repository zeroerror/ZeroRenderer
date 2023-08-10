#pragma once 
#include "EditorUICanvasNode.h"
#include <vector>

using namespace std;

class EditorUICanvas {

public:
	EditorUICanvas(const Rect& rect, const AlignType& alignType);
	~EditorUICanvas();

	void Add(EditorUICanvasNode* node);
	void Remove(EditorUICanvasNode* node);
	void Draw();
	void Foreach(void (*func)(const EditorUICanvasNode* node));
	bool RaycastHitCanvas(const vec2& mousePosition, EditorUICanvasNode*& hitCanvasNode) const;

private:
	EditorUICanvasNode* _root;
	int _id;
	void _Foreach(function<void(EditorUICanvasNode* node)> func) const;

};
