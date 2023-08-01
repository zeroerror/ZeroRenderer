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

class Transform : public Component {

public:
	Transform();
	~Transform();

	vec3 position;
	quat rotation;
	vec3 scale;
	vec3 GetPosition()const;
	void SetPosition(const vec3& newPosition);
	quat GetRotation() const;
	void SetRotation(const quat& newRotation);
	vec3 GetForward() const;
	vec3 GetUp() const;
	vec3 GetRight() const;

private:
	vec3 m_forward;
	vec3 m_up;
	vec3 m_right;

public:
	void SetFather(Transform* father);
	void AddChild(Transform* child);
	void RemoveChild(const Transform& child);
	Transform* GetFather() const;
	Transform* GetChild(int index) const;
	Transform* Find(const string& path);
	int GetChildCount() const;

private:
	Transform* _father;
	vector<Transform*>* _children;
	Transform* _Find(const string& path, Transform* transform) const;
	Transform* _Find(const string& name) const;
	string GetName() const;
};
