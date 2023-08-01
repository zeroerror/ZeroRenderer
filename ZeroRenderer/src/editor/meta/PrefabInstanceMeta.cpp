#include "PrefabInstanceMeta.h"

PrefabInstanceMeta::PrefabInstanceMeta() {
	transformMeta = new TransformMeta();
}

PrefabInstanceMeta::~PrefabInstanceMeta() {
	delete transformMeta;
}