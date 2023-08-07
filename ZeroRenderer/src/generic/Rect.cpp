#include "Rect.h"
#include "AnchorPointType.h"

Rect::Rect() {
	width = 0;
	height = 0;
	x = 0;
	y = 0;
	_anchorPointType = AnchorPointType::Center;
	_RefreshAllPoints();
}

Rect::Rect(float width, float height, float x, float y) {
	this->width = width;
	this->height = height;
	this->x = x;
	this->y = y;
	_anchorPointType = AnchorPointType::Center;
	_RefreshAllPoints();
}

Rect::Rect(float width, float height, float x, float y, const AnchorPointType& anchorPointType) {
	this->width = width;
	this->height = height;
	this->x = x;
	this->y = y;
	this->_anchorPointType = anchorPointType;
	_RefreshAllPoints();
}

Rect::~Rect() {
}

AnchorPointType Rect::GetAnchorPointType() { return _anchorPointType; }
vec2 Rect::GetLeftTop() { return _leftTop; }
vec2 Rect::GetLeftCenter() { return _leftCenter; }
vec2 Rect::GetLeftBottom() { return _leftBottom; }
vec2 Rect::GetCenterTop() { return _centerTop; }
vec2 Rect::GetCenter() { return _center; }
vec2 Rect::GetCenterBottom() { return _centerBottom; }
vec2 Rect::GetRightTop() { return _rightTop; }
vec2 Rect::GetRightCenter() { return _rightCenter; }
vec2 Rect::GetRightBottom() { return _rightBottom; }

void Rect::SetAnchorPointType(const AnchorPointType& anchorPointType) {
	if (this->_anchorPointType == anchorPointType) return;

	vec2 newAnchorPoint = vec2(0, 0);
	if (anchorPointType == AnchorPointType::Center) { newAnchorPoint = GetCenter(); }
	else if (anchorPointType == AnchorPointType::LeftTop) { newAnchorPoint = GetLeftTop(); }
	else if (anchorPointType == AnchorPointType::LeftCenter) { newAnchorPoint = GetLeftCenter(); }
	else if (anchorPointType == AnchorPointType::LeftBottom) { newAnchorPoint = GetLeftBottom(); }
	else if (anchorPointType == AnchorPointType::CenterTop) { newAnchorPoint = GetCenterTop(); }
	else if (anchorPointType == AnchorPointType::CenterBottom) { newAnchorPoint = GetCenterBottom(); }
	else if (anchorPointType == AnchorPointType::RightTop) { newAnchorPoint = GetRightTop(); }
	else if (anchorPointType == AnchorPointType::RightCenter) { newAnchorPoint = GetRightCenter(); }
	else if (anchorPointType == AnchorPointType::RightBottom) { newAnchorPoint = GetRightBottom(); }

	this->_anchorPointType = anchorPointType;
	this->x = newAnchorPoint.x;
	this->y = newAnchorPoint.y;
}

void Rect::SetWidth(const float& width) {
	this->width = width;
	_RefreshAllPoints();
}

void Rect::SetHeight(const float& height) {
	this->height = height;
	_RefreshAllPoints();
}

void Rect::SetWidthHeight(const float& width, const float& height) {
	this->width = width;
	this->height = height;
	_RefreshAllPoints();
}

void Rect::SetX(const float& x) {
	this->x = x;
	_RefreshAllPoints();
}

void Rect::SetX(const float& x, const AnchorPointType& anchorPointType) {
	AnchorPointType apt = _anchorPointType;
	SetAnchorPointType(anchorPointType);
	SetX(x);
	SetAnchorPointType(apt);
	_RefreshAllPoints();
}

void Rect::SetY(const float& y) {
	this->y = y;
	_RefreshAllPoints();
}

void Rect::SetY(const float& y, const AnchorPointType& anchorPointType) {
	AnchorPointType apt = _anchorPointType;
	SetAnchorPointType(anchorPointType);
	SetY(y);
	SetAnchorPointType(apt);
	_RefreshAllPoints();
}

void Rect::SetXY(const float& x, const float& y) {
	this->x = x;
	this->y = y;
	_RefreshAllPoints();
}

void Rect::SetXY(const vec2& xy) {
	this->x = xy.x;
	this->y = xy.y;
	_RefreshAllPoints();
}

void Rect::SetXY(const float& x, const float& y, const AnchorPointType& anchorPointType) {
	AnchorPointType apt = _anchorPointType;
	SetAnchorPointType(anchorPointType);
	SetXY(x, y);
	SetAnchorPointType(apt);
	_RefreshAllPoints();
}

void Rect::SetXY(const vec2& xy, const AnchorPointType& anchorPointType) {
	AnchorPointType apt = this->_anchorPointType;
	SetAnchorPointType(anchorPointType);
	SetXY(xy.x, xy.y);
	SetAnchorPointType(apt);
	_RefreshAllPoints();
}

void Rect::SetXY_AsLeftTop(const vec2& leftTop) {
	AnchorPointType apt = _anchorPointType;
	SetAnchorPointType(AnchorPointType::LeftTop);
	SetXY(leftTop);
	SetAnchorPointType(apt);
}

void Rect::SetXY_AsLeftCenter(const vec2& leftCenter) {
	AnchorPointType apt = _anchorPointType;
	SetAnchorPointType(AnchorPointType::LeftCenter);
	SetXY(leftCenter);
	SetAnchorPointType(apt);
}

void Rect::SetXY_AsLeftBottom(const vec2& leftBottom) {
	AnchorPointType apt = _anchorPointType;
	SetAnchorPointType(AnchorPointType::LeftBottom);
	SetXY(leftBottom);
	SetAnchorPointType(apt);
}

void Rect::SetXY_AsCenterTop(const vec2& centerTop) {
	AnchorPointType apt = _anchorPointType;
	SetAnchorPointType(AnchorPointType::CenterTop);
	SetXY(centerTop);
	SetAnchorPointType(apt);
}

void Rect::SetXY_AsCenter(const vec2& center) {
	AnchorPointType apt = _anchorPointType;
	SetAnchorPointType(AnchorPointType::Center);
	SetXY(center);
	SetAnchorPointType(apt);
}

void Rect::SetXY_AsCenterBottom(const vec2& centerBottom) {
	AnchorPointType apt = _anchorPointType;
	SetAnchorPointType(AnchorPointType::CenterBottom);
	SetXY(centerBottom);
	SetAnchorPointType(apt);
}

void Rect::SetXY_AsRightTop(const vec2& rightTop) {
	AnchorPointType apt = _anchorPointType;
	SetAnchorPointType(AnchorPointType::RightTop);
	SetXY(rightTop);
	SetAnchorPointType(apt);
}

void Rect::SetXY_AsRightCenter(const vec2& rightCenter) {
	AnchorPointType apt = _anchorPointType;
	SetAnchorPointType(AnchorPointType::RightCenter);
	SetXY(rightCenter);
	SetAnchorPointType(apt);
}

void Rect::SetXY_AsRightBottom(const vec2& rightBottom) {
	AnchorPointType apt = _anchorPointType;
	SetAnchorPointType(AnchorPointType::RightBottom);
	SetXY(rightBottom);
	SetAnchorPointType(apt);
}

void Rect::_RefreshAllPoints() {
	if (_anchorPointType == AnchorPointType::Center) {
		_leftTop = vec2(x - width / 2, y - height / 2);
		_leftCenter = vec2(x - width / 2, y);
		_leftBottom = vec2(x - width / 2, y + height / 2);
		_centerTop = vec2(x, y - height / 2);
		_center = vec2(x, y);
		_centerBottom = vec2(x, y + height / 2);
		_rightTop = vec2(x + width / 2, y - height / 2);
		_rightCenter = vec2(x + width / 2, y);
		_rightBottom = vec2(x + width / 2, y + height / 2);
		return;
	}

	if (_anchorPointType == AnchorPointType::LeftTop) {
		_leftTop = vec2(x, y);
		_leftCenter = vec2(x, y + height / 2);
		_leftBottom = vec2(x, y + height);
		_centerTop = vec2(x + width / 2, y);
		_center = vec2(x + width / 2, y + height / 2);
		_centerBottom = vec2(x + width / 2, y + height);
		_rightTop = vec2(x + width, y);
		_rightCenter = vec2(x + width, y + height / 2);
		_rightBottom = vec2(x + width, y + height);
		return;
	}

	if (_anchorPointType == AnchorPointType::LeftCenter) {
		_leftTop = vec2(x, y - height / 2);
		_leftCenter = vec2(x, y);
		_leftBottom = vec2(x, y + height / 2);
		_centerTop = vec2(x + width / 2, y - height / 2);
		_center = vec2(x + width / 2, y);
		_centerBottom = vec2(x + width / 2, y + height / 2);
		_rightTop = vec2(x + width, y - height / 2);
		_rightCenter = vec2(x + width, y);
		_rightBottom = vec2(x + width, y + height / 2);
		return;
	}

	if (_anchorPointType == AnchorPointType::LeftBottom) {
		_leftTop = vec2(x, y - height);
		_leftCenter = vec2(x, y - height / 2);
		_leftBottom = vec2(x, y);
		_centerTop = vec2(x + width / 2, y - height);
		_center = vec2(x + width / 2, y - height / 2);
		_centerBottom = vec2(x + width / 2, y);
		_rightTop = vec2(x + width, y - height);
		_rightCenter = vec2(x + width, y - height / 2);
		_rightBottom = vec2(x + width, y);
		return;
	}

	if (_anchorPointType == AnchorPointType::CenterTop) {
		_leftTop = vec2(x - width / 2, y);
		_leftCenter = vec2(x - width / 2, y + height / 2);
		_leftBottom = vec2(x - width / 2, y + height);
		_centerTop = vec2(x, y);
		_center = vec2(x, y + height / 2);
		_centerBottom = vec2(x, y + height);
		_rightTop = vec2(x + width / 2, y);
		_rightCenter = vec2(x + width / 2, y + height / 2);
		_rightBottom = vec2(x + width / 2, y + height);
		return;
	}

	if (_anchorPointType == AnchorPointType::CenterBottom) {
		_leftTop = vec2(x - width / 2, y - height);
		_leftCenter = vec2(x - width / 2, y - height / 2);
		_leftBottom = vec2(x - width / 2, y);
		_centerTop = vec2(x, y - height);
		_center = vec2(x, y - height / 2);
		_centerBottom = vec2(x, y);
		_rightTop = vec2(x + width / 2, y - height);
		_rightCenter = vec2(x + width / 2, y - height / 2);
		_rightBottom = vec2(x + width / 2, y);
		return;
	}

	if (_anchorPointType == AnchorPointType::RightTop) {
		_leftTop = vec2(x - width, y);
		_leftCenter = vec2(x - width, y + height / 2);
		_leftBottom = vec2(x - width, y + height);
		_centerTop = vec2(x - width / 2, y);
		_center = vec2(x - width / 2, y + height / 2);
		_centerBottom = vec2(x - width / 2, y + height);
		_rightTop = vec2(x, y);
		_rightCenter = vec2(x, y + height / 2);
		_rightBottom = vec2(x, y + height);
		return;
	}

	if (_anchorPointType == AnchorPointType::RightCenter) {
		_leftTop = vec2(x - width, y - height / 2);
		_leftCenter = vec2(x - width, y);
		_leftBottom = vec2(x - width, y + height / 2);
		_centerTop = vec2(x - width / 2, y - height / 2);
		_center = vec2(x - width / 2, y);
		_centerBottom = vec2(x - width / 2, y + height / 2);
		_rightTop = vec2(x, y - height / 2);
		_rightCenter = vec2(x, y);
		_rightBottom = vec2(x, y + height / 2);
		return;
	}

	if (_anchorPointType == AnchorPointType::RightBottom) {
		_leftTop = vec2(x - width, y - height);
		_leftCenter = vec2(x - width, y - height / 2);
		_leftBottom = vec2(x - width, y);
		_centerTop = vec2(x - width / 2, y - height);
		_center = vec2(x - width / 2, y - height / 2);
		_centerBottom = vec2(x - width / 2, y);
		_rightTop = vec2(x, y - height);
		_rightCenter = vec2(x, y - height / 2);
		_rightBottom = vec2(x, y);
		return;
	}
}