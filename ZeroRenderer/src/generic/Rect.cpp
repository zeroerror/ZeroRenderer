#include "Rect.h"

Rect::Rect() {
    width = 0;
    height = 0;
    x = 0;
    y = 0;
    anchorPointType = AnchorPointType::Center;
}

Rect::Rect(float width, float height, float x, float y) {
    this->width = width;
    this->height = height;
    this->x = x;
    this->y = y;
    anchorPointType = AnchorPointType::Center;
}

Rect::Rect(float width, float height, float x, float y, AnchorPointType anchorPointType) {
    this->width = width;
    this->height = height;
    this->x = x;
    this->y = y;
    this->anchorPointType = anchorPointType;
}

Rect::~Rect() {
}

vec2 Rect::GetLeftTop() {
    if(anchorPointType == AnchorPointType::Center) return vec2(x - width / 2, y - height / 2);
    if(anchorPointType == AnchorPointType::LeftTop) return vec2(x, y);
    if(anchorPointType == AnchorPointType::LeftCenter) return vec2(x, y - height / 2);
    if(anchorPointType == AnchorPointType::LeftBottom) return vec2(x, y - height);
    if(anchorPointType == AnchorPointType::CenterTop) return vec2(x - width / 2, y);
    if(anchorPointType == AnchorPointType::CenterBottom) return vec2(x - width / 2, y - height);
    if(anchorPointType == AnchorPointType::RightTop) return vec2(x - width, y);
    if(anchorPointType == AnchorPointType::RightCenter) return vec2(x - width, y - height / 2);
    if(anchorPointType == AnchorPointType::RightBottom) return vec2(x - width, y - height);
}

vec2 Rect::GetLeftCenter() {
    if(anchorPointType == AnchorPointType::Center) return vec2(x - width / 2, y);
    if(anchorPointType == AnchorPointType::LeftTop) return vec2(x, y + height / 2);
    if(anchorPointType == AnchorPointType::LeftCenter) return vec2(x, y);
    if(anchorPointType == AnchorPointType::LeftBottom) return vec2(x, y - height / 2);
    if(anchorPointType == AnchorPointType::CenterTop) return vec2(x - width / 2, y + height / 2);
    if(anchorPointType == AnchorPointType::CenterBottom) return vec2(x - width / 2, y - height / 2);
    if(anchorPointType == AnchorPointType::RightTop) return vec2(x - width, y + height / 2);
    if(anchorPointType == AnchorPointType::RightCenter) return vec2(x - width, y);
    if(anchorPointType == AnchorPointType::RightBottom) return vec2(x - width, y - height / 2);
}

vec2 Rect::GetLeftBottom() {
    if(anchorPointType == AnchorPointType::Center) return vec2(x - width / 2, y + height / 2);
    if(anchorPointType == AnchorPointType::LeftTop) return vec2(x, y + height);
    if(anchorPointType == AnchorPointType::LeftCenter) return vec2(x, y + height / 2);
    if(anchorPointType == AnchorPointType::LeftBottom) return vec2(x, y);
    if(anchorPointType == AnchorPointType::CenterTop) return vec2(x - width / 2, y + height);
    if(anchorPointType == AnchorPointType::CenterBottom) return vec2(x - width / 2, y);
    if(anchorPointType == AnchorPointType::RightTop) return vec2(x - width, y + height);
    if(anchorPointType == AnchorPointType::RightCenter) return vec2(x - width, y + height / 2);
    if(anchorPointType == AnchorPointType::RightBottom) return vec2(x - width, y);
}

vec2 Rect::GetCenterTop() {
    if(anchorPointType == AnchorPointType::Center) return vec2(x, y - height / 2);
    if(anchorPointType == AnchorPointType::LeftTop) return vec2(x + width / 2, y);
    if(anchorPointType == AnchorPointType::LeftCenter) return vec2(x + width / 2, y - height / 2);
    if(anchorPointType == AnchorPointType::LeftBottom) return vec2(x + width / 2, y - height);
    if(anchorPointType == AnchorPointType::CenterTop) return vec2(x, y);
    if(anchorPointType == AnchorPointType::CenterBottom) return vec2(x, y - height);
    if(anchorPointType == AnchorPointType::RightTop) return vec2(x - width / 2, y);
    if(anchorPointType == AnchorPointType::RightCenter) return vec2(x - width / 2, y - height / 2);
    if(anchorPointType == AnchorPointType::RightBottom) return vec2(x - width / 2, y - height);
}

vec2 Rect::GetCenter() {
    if(anchorPointType == AnchorPointType::Center) return vec2(x, y);
    if(anchorPointType == AnchorPointType::LeftTop) return vec2(x + width / 2, y + height / 2);
    if(anchorPointType == AnchorPointType::LeftCenter) return vec2(x + width / 2, y);
    if(anchorPointType == AnchorPointType::LeftBottom) return vec2(x + width / 2, y - height / 2);
    if(anchorPointType == AnchorPointType::CenterTop) return vec2(x, y + height / 2);
    if(anchorPointType == AnchorPointType::CenterBottom) return vec2(x, y - height / 2);
    if(anchorPointType == AnchorPointType::RightTop) return vec2(x - width / 2, y + height / 2);
    if(anchorPointType == AnchorPointType::RightCenter) return vec2(x - width / 2, y);
    if(anchorPointType == AnchorPointType::RightBottom) return vec2(x - width / 2, y - height / 2);
}

vec2 Rect::GetCenterBottom() {
    if(anchorPointType == AnchorPointType::Center) return vec2(x, y + height / 2);
    if(anchorPointType == AnchorPointType::LeftTop) return vec2(x + width / 2, y + height);
    if(anchorPointType == AnchorPointType::LeftCenter) return vec2(x + width / 2, y + height / 2);
    if(anchorPointType == AnchorPointType::LeftBottom) return vec2(x + width / 2, y);
    if(anchorPointType == AnchorPointType::CenterTop) return vec2(x, y + height);
    if(anchorPointType == AnchorPointType::CenterBottom) return vec2(x, y);
    if(anchorPointType == AnchorPointType::RightTop) return vec2(x - width / 2, y + height);
    if(anchorPointType == AnchorPointType::RightCenter) return vec2(x - width / 2, y + height / 2);
    if(anchorPointType == AnchorPointType::RightBottom) return vec2(x - width / 2, y);
}

vec2 Rect::GetRightTop() {
    if(anchorPointType == AnchorPointType::Center) return vec2(x + width / 2, y - height / 2);
    if(anchorPointType == AnchorPointType::LeftTop) return vec2(x + width, y);
    if(anchorPointType == AnchorPointType::LeftCenter) return vec2(x + width, y - height / 2);
    if(anchorPointType == AnchorPointType::LeftBottom) return vec2(x + width, y - height);
    if(anchorPointType == AnchorPointType::CenterTop) return vec2(x + width / 2, y);
    if(anchorPointType == AnchorPointType::CenterBottom) return vec2(x + width / 2, y - height);
    if(anchorPointType == AnchorPointType::RightTop) return vec2(x, y);
    if(anchorPointType == AnchorPointType::RightCenter) return vec2(x, y - height / 2);
    if(anchorPointType == AnchorPointType::RightBottom) return vec2(x, y - height);
}

vec2 Rect::GetRightCenter() {
    if(anchorPointType == AnchorPointType::Center) return vec2(x + width / 2, y);
    if(anchorPointType == AnchorPointType::LeftTop) return vec2(x + width, y + height / 2);
    if(anchorPointType == AnchorPointType::LeftCenter) return vec2(x + width, y);
    if(anchorPointType == AnchorPointType::LeftBottom) return vec2(x + width, y - height / 2);
    if(anchorPointType == AnchorPointType::CenterTop) return vec2(x + width / 2, y + height / 2);
    if(anchorPointType == AnchorPointType::CenterBottom) return vec2(x + width / 2, y - height / 2);
    if(anchorPointType == AnchorPointType::RightTop) return vec2(x, y + height / 2);
    if(anchorPointType == AnchorPointType::RightCenter) return vec2(x, y);
    if(anchorPointType == AnchorPointType::RightBottom) return vec2(x, y - height / 2);
}

vec2 Rect::GetRightBottom() {
    if(anchorPointType == AnchorPointType::Center) return vec2(x + width / 2, y + height / 2);
    if(anchorPointType == AnchorPointType::LeftTop) return vec2(x + width, y + height);
    if(anchorPointType == AnchorPointType::LeftCenter) return vec2(x + width, y + height / 2);
    if(anchorPointType == AnchorPointType::LeftBottom) return vec2(x + width, y);
    if(anchorPointType == AnchorPointType::CenterTop) return vec2(x + width / 2, y + height);
    if(anchorPointType == AnchorPointType::CenterBottom) return vec2(x + width / 2, y);
    if(anchorPointType == AnchorPointType::RightTop) return vec2(x, y + height);
    if(anchorPointType == AnchorPointType::RightCenter) return vec2(x, y + height / 2);
    if(anchorPointType == AnchorPointType::RightBottom) return vec2(x, y);
}