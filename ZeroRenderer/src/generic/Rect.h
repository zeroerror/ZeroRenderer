#pragma once 
#include "AnchorPointType.h"

#include <glm/glm.hpp>

using namespace std;
using namespace glm;

class Rect{

public:
    float width;
    float height;
    float x;
    float y;
    AnchorPointType anchorPointType;

    Rect();
    Rect(float width, float height, float x, float y);
    Rect(float width, float height, float x, float y, AnchorPointType anchorPointType);
    ~Rect();

    vec2 GetLeftTop();
    vec2 GetLeftCenter();
    vec2 GetLeftBottom();
    vec2 GetCenterTop();
    vec2 GetCenter();
    vec2 GetCenterBottom();
    vec2 GetRightTop();
    vec2 GetRightCenter();
    vec2 GetRightBottom();

};
