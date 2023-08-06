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

    Rect();
    Rect(float width, float height, float x, float y);
    Rect(float width, float height, float x, float y, const AnchorPointType& anchorPointType);
    ~Rect();

    AnchorPointType GetAnchorPointType();
    vec2 GetLeftTop();
    vec2 GetLeftCenter();
    vec2 GetLeftBottom();
    vec2 GetCenterTop();
    vec2 GetCenter();
    vec2 GetCenterBottom();
    vec2 GetRightTop();
    vec2 GetRightCenter();
    vec2 GetRightBottom();

    void SetAnchorPointType(const AnchorPointType& anchorPointType);
    void SetWidth(const float& width);
    void SetHeight(const float& height);
    void SetWidthHeight(const float& width, const float& height);
    void SetX(const float& x);
    void SetX(const float& x, const AnchorPointType& byAnchorPointType);
    void SetY(const float& y);
    void SetY(const float& y, const AnchorPointType& byAnchorPointType);
    void SetXY(const float& x, const float& y);
    void SetXY(const vec2& xy);
    void SetXY(const float& x, const float& y, const AnchorPointType& byAnchorPointType);
    void SetXY(const vec2& xy, const AnchorPointType& byAnchorPointType);
    
private:
    vec2 _leftTop;
    vec2 _leftCenter;
    vec2 _leftBottom;
    vec2 _centerTop;
    vec2 _center;
    vec2 _centerBottom;
    vec2 _rightTop;
    vec2 _rightCenter;
    vec2 _rightBottom;
    AnchorPointType _anchorPointType;

    void _RefreshAllPoints();

};
