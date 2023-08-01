#pragma once
#include <string>
#include "TransformMeta.h"

using namespace std;

class PrefabInstanceMeta {

public:
	PrefabInstanceMeta();
	~PrefabInstanceMeta();

	TransformMeta* transformMeta;
	string guid;
	int gid;
	string name;
};

