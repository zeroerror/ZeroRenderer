#pragma once
#include "Rect.h"
#include "AlignType.h"
#include <vector>
#include <functional>

using namespace std;

class EditorUICanvasNode
{

public:
	typedef function<void(const Rect &rect)> Callback;
	Callback drawCb;
	Rect rect;
	AlignType alignType;
	string name;

	/**
	 * @brief Construct a new Editor UICanvas Node object
	 */
	EditorUICanvasNode(const Rect &rect, const AlignType &alignType);
	~EditorUICanvasNode();

	void Draw();

public:
	EditorUICanvasNode *father;
	EditorUICanvasNode *child;

	/**
	 * @brief Check if the position is in the canvas
	 */
	bool IsInCanvas(const vec2 &pos);
};
