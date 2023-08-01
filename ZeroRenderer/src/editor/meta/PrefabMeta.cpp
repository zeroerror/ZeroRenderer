#include "PrefabMeta.h"

PrefabMeta::PrefabMeta() {
	transformMeta = AddComponentMeta<TransformMeta>();
};

PrefabMeta::~PrefabMeta() {
	for (auto comMeta : componentMetas) {
		delete comMeta;
	}
};