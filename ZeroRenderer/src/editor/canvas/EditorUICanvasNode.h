#pragma once 
#include "Rect.h"
#include "AlignType.h"

#include <vector>

using namespace std;

class EditorUICanvasNode{

public:
    EditorUICanvasNode(const Rect& rect, AlignType alignType);
    ~EditorUICanvasNode();

    Rect rect;
    AlignType alignType;

private:
    EditorUICanvasNode* _father;
    vector<EditorUICanvasNode*> _children;

    void Add(EditorUICanvasNode* node);
    void Remove(EditorUICanvasNode* node);

};
