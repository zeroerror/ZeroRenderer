#include "EditorUICanvas.h"

EditorUICanvas::EditorUICanvas(EditorUICanvasNode root) {
    _root = root;
}

EditorUICanvas::~EditorUICanvas() {
    delete _root;
}

void EditorUICanvas::Add(EditorUICanvasNode* node){
    _root->Add(node);
}

void EditorUICanvas::Remove(EditorUICanvasNode* node){
    _root->Remove(node);
}

unsigned int EditorUICanvas::GetChildCount(){
    return _childrens.size();
}