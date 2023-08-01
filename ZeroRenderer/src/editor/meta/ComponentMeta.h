#pragma once
#include "ComponentType.h"

class TransformMeta;
class GameObjectMeta;

class ComponentMeta
{
public:
	ComponentType_ componentType;
	GameObjectMeta* gameObjectMeta;
	TransformMeta* transformMeta;
};

