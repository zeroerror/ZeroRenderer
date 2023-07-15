#include "EditorDefaultConfig.h"

string EditorDefaultConfig::_defaultScenePath = "asset/DefaultScene.scene";
string EditorDefaultConfig::_defaultComponentEndStr = "---componentEnd";
string EditorDefaultConfig::_defaultGameObjectEndStr = "======gameObjectEnd";

const string& EditorDefaultConfig::DefaultScenePath() { return _defaultScenePath; }
const string& EditorDefaultConfig::DefaultComponentEndStr() { return _defaultComponentEndStr; }
const string& EditorDefaultConfig::DefaultGameObjectEndStr() { return _defaultGameObjectEndStr; }