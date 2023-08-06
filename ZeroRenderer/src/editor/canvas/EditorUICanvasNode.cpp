#include "EditorUICanvasNode.h"

EditorUICanvasNode::EditorUICanvasNode(const Rect& rect, AlignType alignType){
    this->rect = rect;
    this->alignType = alignType;
}

~EditorUICanvasNode::EditorUICanvasNode(){
    for (unsigned int i = 0; i < _children->size(); i++) {
        delete _children->at(i);
    }
    
    if(_father != nullptr) delete _father;
    if(_children != nullptr) delete _children;
}
    
void EditorUICanvasNode::Add(EditorUICanvasNode* node){
	this->_children->push_back(node);
	child->_father = this;

    size_t size = this->_children->size();
    size_t index = size - 1;
    if(index < 0){
        if(alignType == AlignType::Left){
            node->rect.SetXY(this->rect.GetLeftTop(), AnchorPointType::LeftTop);
            return;
        }

        cout << "############# Error: EditorUICanvasNode::Add(EditorUICanvasNode* node) AlignType Not Handle" << endl;
        return;
    }

    EditorUICanvasNode* last = this->_children->at(index);
    AnchorPointType anchorPointType = this->rect.anchorPointType;
    float width = this->rect.width;
    float height = this->rect.height;

    if(anchorPointType == AnchorPointType::LeftTop){
        vec2 lastRightTop = last->rect.GetRightTop();
        node->rect.SetXY(lastRightTop, AnchorPointType::LeftTop);
    }
}

void EditorUICanvasNode::Remove(EditorUICanvasNode* node){
    unsigned int index = 0;
	for (unsigned int i = 0; i < _children->size(); i++) {
        if (_children->at(i) == node) {
            index = i;
            _children->erase(_children->begin() + i);
            break;
        }
	}

    AnchorPointType anchorPointType = this->rect.anchorPointType;
	for (unsigned int i = 0; i < _children->size(); i++) {
        if(i < index){

        }else{

        }
    }
}


