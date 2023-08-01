#include "TransformMeta.h"

TransformMeta::TransformMeta() {
	componentType = ComponentType_Transform;
	position = vec3(0, 0, 0);
	rotation = quat(1, 0, 0, 0);
	scale = vec3(1, 1, 1);

	gid = 0;
	fatherGID = 0;
	childrenGIDs = new vector<int>();
	children = new vector<TransformMeta*>();
}

TransformMeta::~TransformMeta() {
}

void TransformMeta::SetFather(TransformMeta* father) {
	this->father = father;
	this->fatherGID = father->gid;
	father->childrenGIDs->push_back(gid);
	father->children->push_back(this);
}

void TransformMeta::AddChild(TransformMeta* child) {
	this->childrenGIDs->push_back(child->gid);
	this->children->push_back(child);
	child->father = this;
	child->fatherGID = gid;
}

void TransformMeta::RemoveChild(const TransformMeta& child) {
	for (int i = 0; i < children->size(); i++) {
		if (children->at(i)->gid == child.gid) {
			children->erase(children->begin() + i);
			childrenGIDs->erase(childrenGIDs->begin() + i);
			break;
		}
	}
}
