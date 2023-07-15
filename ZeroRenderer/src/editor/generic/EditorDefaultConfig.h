#pragma once 
#include <string>

using namespace std;

class EditorDefaultConfig
{

public:
	static const string& DefaultScenePath();
	static const string& DefaultComponentEndStr();
	static const string& DefaultGameObjectEndStr();

private:
	static string _defaultScenePath;
	static string _defaultComponentEndStr;
	static string _defaultGameObjectEndStr;

};
