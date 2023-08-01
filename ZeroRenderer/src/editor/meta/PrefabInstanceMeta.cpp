#include "PrefabInstanceMeta.h"

PrefabInstanceMeta::PrefabInstanceMeta() {
	gameObjectMeta = new GameObjectMeta();
}

PrefabInstanceMeta::~PrefabInstanceMeta() {
	delete gameObjectMeta;
}