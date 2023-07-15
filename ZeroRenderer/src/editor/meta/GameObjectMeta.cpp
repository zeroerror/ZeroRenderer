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

void GameObjectMeta::ToGameObject(GameObject& gameObject) {
	gameObject.name = name;
	transformMeta.ToTransform(gameObject.transform());
	for (ComponentMeta* componentMeta : componentMetas) {
		if (componentMeta->componentType == ComponentType_Camera) {
			Camera* camera = gameObject.AddComponent<Camera>();
			CameraMeta* cameraMeta = static_cast<CameraMeta*>(componentMeta);
			cameraMeta->ToCamera(camera);
		}
	}
}