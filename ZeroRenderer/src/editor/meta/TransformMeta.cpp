#include "TransformMeta.h"

TransformMeta::TransformMeta() {
	componentType = ComponentType_Transform;
	position = vec3(0, 0, 0);
	rotation = quat(1, 0, 0, 0);
	scale = vec3(1, 1, 1);
}

TransformMeta::~TransformMeta() {

}