#pragma once
#include <string>
#include <vector>
#include <glm/ext/vector_float4.hpp>

using namespace std;

class ObjMeta {

public:
	string guid;
    vector<string> materialGUIDs;
    vector<string> meshNames;
};

