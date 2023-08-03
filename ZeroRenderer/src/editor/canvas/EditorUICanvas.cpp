#include "EditorUICanvas.h"

EditorUICanvas::EditorUICanvas(EditorUICanvasNode root) {
    _root = root;
    _childrens = vector<EditorUICanvasNode*>();
}

EditorUICanvas::~EditorUICanvas() {
    delete _root;
    for (int i = 0; i < _childrens.size(); i++) {
        delete _childrens[i];
    }
    delete _childrens;
}

void EditorUICanvas::addChild(EditorUICanvasNode* node) {
    _childrens.push_back(node);
}

