#include "GameObjectMeta.h"
#include "CameraMeta.h"

GameObjectMeta::GameObjectMeta() {
	componentMetas = vector<ComponentMeta*>();
	transformMeta = new TransformMeta();
}

GameObjectMeta::~GameObjectMeta() {
	for (ComponentMeta* componentMeta : componentMetas) {
		delete componentMeta;
	}
	componentMetas.clear();
	delete transformMeta;
}
