#pragma once
#include <string>
#include "TransformMeta.h"

using namespace std;

class PrefabInstanceMeta {

public:
	PrefabInstanceMeta();
	~PrefabInstanceMeta();

	int gid;
	string guid;
	string name;
	TransformMeta* transformMeta;
};

