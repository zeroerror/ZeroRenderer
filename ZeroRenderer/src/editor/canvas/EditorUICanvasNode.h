#pragma once 
#include "Rect.h"

#include <vector>

using namespace std;

class EditorUICanvasNode{
public:
    EditorUICanvasNode father;
    vector<Rect> _children;

    EditorUICanvasNode(Rect rect);
    ~EditorUICanvasNode();

};
