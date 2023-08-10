#include "ZeroMathUtil.h"

int ZeroMathUtil::_Clamp(const int& v, const int& min, const int& max) {
	return v<min ? min : v>max ? max : v;
}