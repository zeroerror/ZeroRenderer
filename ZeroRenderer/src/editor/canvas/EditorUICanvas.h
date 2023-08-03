#pragma once 
#include "EditorUICanvasNode.h"
#include <vector>

using namespace std;

class EditorUICanvas{

public:
    EditorUICanvas(EditorUICanvasNode root);
    ~EditorUICanvas();

private:
    EditorUICanvasNode* _root;
    vector<EditorUICanvasNode*> _childrens;

    int _id;

public: 
    void Add(EditorUICanvasNode node);
    void Remove(EditorUICanvasNode node);

    unsigned int GetChildCount();
    

};
