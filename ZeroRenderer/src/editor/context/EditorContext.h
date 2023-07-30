#pragma once
#include "SceneMeta.h"

class EditorContext {

public:
	SceneMeta GetCurrentSceneMeta();
	SceneMeta SetCurrentSceneMeta();
private:
	SceneMeta _currentSceneMeta;

};