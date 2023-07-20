#pragma once 
#include <string>

using namespace std;

class EditorDefaultConfig
{

public:
	static const string& DefaultScenePath();
	static const string& DefaultShaderPath();

	static const string& DefaultComponentStartStr();
	static const string& DefaultGameObjectStartStr();
	static const string& DefaultPrefabInstanceStartStr();
	static const string& DefaultSceneStartStr();
	static const string& DefaultAllGameObjectsStartStr();
	static const string& DefaultAllPrefabInstancesStartStr();

	static const string& DefaultComponentEndStr();
	static const string& DefaultGameObjectEndStr();
	static const string& DefaultPrefabInstanceEndStr();
	static const string& DefaultSceneEndStr();
	static const string& DefaultAllGameObjectsEndStr();
	static const string& DefaultAllPrefabInstancesEndStr();

private:
	static string _defaultScenePath;
	static string _defaultShaderPath;

	static string _defaultSceneStartStr;
	static string _defaultComponentStartStr;
	static string _defaultGameObjectStartStr;
	static string _defaultPrefabInstanceStartStr;
	static string _defaultAllGameObjectsStartStr;
	static string _defaultAllPrefabInstancesStartStr;

	static string _defaultSceneEndStr;
	static string _defaultComponentEndStr;
	static string _defaultGameObjectEndStr;
	static string _defaultPrefabInstanceEndStr;
	static string _defaultAllGameObjecntsEndStr;
	static string _defaultAllPrefabInstancesEndStr;

};
