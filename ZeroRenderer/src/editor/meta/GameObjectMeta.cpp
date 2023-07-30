#include "GameObjectMeta.h"
#include "CameraMeta.h"

GameObjectMeta::GameObjectMeta() {
	componentMetas = vector<ComponentMeta*>();
}

GameObjectMeta::~GameObjectMeta() {
	for (ComponentMeta* componentMeta : componentMetas) {
		delete componentMeta;
	}
	componentMetas.clear();
}
