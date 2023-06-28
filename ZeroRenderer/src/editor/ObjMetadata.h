#pragma once
#include <string>
#include <vector>
#include <glm/ext/vector_float4.hpp>

using namespace std;

class ObjMetadata {

public:
	ObjMetadata();
	~ObjMetadata();

	string guid;
    vector<string> materialGUIDs;
    vector<string> meshNames;

	void SerializeTo(const string& path);
	void DeserializeFrom(const string& path);

};

