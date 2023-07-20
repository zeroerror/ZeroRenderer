#include "EditorDefaultConfig.h"

string EditorDefaultConfig::_defaultScenePath = "asset/DefaultScene.scene";
string EditorDefaultConfig::_defaultShaderPath = "asset/shader/Default.shader";

string EditorDefaultConfig::_defaultComponentStartStr = "--ComponentStart:";
string EditorDefaultConfig::_defaultGameObjectStartStr = "====GameObjectStart:";
string EditorDefaultConfig::_defaultPrefabInstanceStartStr = "====PrefabInstanceStart:";
string EditorDefaultConfig::_defaultSceneStartStr = "########SceneStart:";
string EditorDefaultConfig::_defaultAllGameObjectsStartStr = "########AllGameObjectsStart:";
string EditorDefaultConfig::_defaultAllPrefabInstancesStartStr = "########AllPrefabInstancesStart:";

string EditorDefaultConfig::_defaultComponentEndStr = "--ComponentEnd";
string EditorDefaultConfig::_defaultGameObjectEndStr = "====GameObjectEnd";
string EditorDefaultConfig::_defaultPrefabInstanceEndStr = "====PrefabInstanceEnd";
string EditorDefaultConfig::_defaultSceneEndStr = "########SceneEnd";
string EditorDefaultConfig::_defaultAllGameObjecntsEndStr = "########AllGameObjectsEnd";
string EditorDefaultConfig::_defaultAllPrefabInstancesEndStr = "########AllPrefabInstancesEnd";

const string& EditorDefaultConfig::DefaultScenePath() { return _defaultScenePath; }
const string& EditorDefaultConfig::DefaultShaderPath() { return _defaultShaderPath; }

const string& EditorDefaultConfig::DefaultComponentStartStr() { return _defaultComponentStartStr; }
const string& EditorDefaultConfig::DefaultGameObjectStartStr() { return _defaultGameObjectStartStr; }
const string& EditorDefaultConfig::DefaultPrefabInstanceStartStr() { return _defaultPrefabInstanceStartStr; }
const string& EditorDefaultConfig::DefaultSceneStartStr() { return _defaultSceneStartStr; }
const string& EditorDefaultConfig::DefaultAllGameObjectsStartStr() { return _defaultAllGameObjectsStartStr; }
const string& EditorDefaultConfig::DefaultAllPrefabInstancesStartStr() { return _defaultAllPrefabInstancesStartStr; }

const string& EditorDefaultConfig::DefaultComponentEndStr() { return _defaultComponentEndStr; }
const string& EditorDefaultConfig::DefaultGameObjectEndStr() { return _defaultGameObjectEndStr; }
const string& EditorDefaultConfig::DefaultPrefabInstanceEndStr() { return _defaultPrefabInstanceEndStr; }
const string& EditorDefaultConfig::DefaultSceneEndStr() { return _defaultSceneEndStr; }
const string& EditorDefaultConfig::DefaultAllGameObjectsEndStr() { return _defaultAllGameObjecntsEndStr; }
const string& EditorDefaultConfig::DefaultAllPrefabInstancesEndStr() { return _defaultAllPrefabInstancesEndStr; }