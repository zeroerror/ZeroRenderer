#pragma once
#include <string>
#include "TransformMeta.h"

using namespace std;

class PrefabInstanceMeta {

public:
	PrefabInstanceMeta();

	TransformMeta transformMeta;
	string guid;
};

