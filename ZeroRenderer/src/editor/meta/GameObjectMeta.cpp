#include "GameObjectMeta.h"
#include "TransformMeta.h"

GameObjectMeta::GameObjectMeta() {
	componentMetas = vector<ComponentMeta*>();
	transformMeta = AddComponentMeta<TransformMeta>();
}

GameObjectMeta::~GameObjectMeta() {
	for (ComponentMeta* componentMeta : componentMetas) {
		delete componentMeta;
	}
	componentMetas.clear();
	//delete transformMeta; // todo bug cant delete
}
