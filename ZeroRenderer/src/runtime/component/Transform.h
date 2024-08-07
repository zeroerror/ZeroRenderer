#pragma once

class Component;
class GameObject;
#include "GameObject.h"
#include "Component.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <sstream>

using namespace std;
using namespace glm;

class Transform : public Component
{

public:
	Transform();
	~Transform();

	vec3 GetPosition() const;
	void SetPosition(const vec3 &newPosition);
	quat GetRotation() const;
	void SetRotation(const quat &newRotation);
	void SetRotation(const vec3 &eulerAngles);

	vec3 GetScale() const;
	void SetScale(const vec3 &newScale);
	vec3 GetForward() const;
	vec3 GetUp() const;
	vec3 GetRight() const;

private:
	vec3 m_forward;
	vec3 m_up;
	vec3 m_right;

	vec3 _position;
	quat _rotation;
	vec3 _scale;

public:
	void SetFather(Transform *father);
	void AddChild(Transform *child);
	void RemoveChild(const Transform &child);
	Transform *GetFather() const;
	Transform *GetChild(int index) const;
	void ForEachChild(void (*func)(Transform *transform));
	Transform *Find(const string &path);
	int GetChildCount() const;

	int fatherGID_forSerialize;
	vector<int> childrenGIDs_forSerialize;

private:
	Transform *_father;
	vector<Transform *> *_children;
	Transform *_Find(const string &path, Transform *transform) const;
	Transform *_Find(const string &name) const;
	string GetName() const;
};
