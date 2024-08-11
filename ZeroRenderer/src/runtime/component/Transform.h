#pragma once

class Component;
class GameObject;
#include "GameObject.h"
#include "Component.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <sstream>
#include <functional>

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
	/**
	 * @brief 设置父节点. 会同时更新新旧父节点的子节点列表
	 */
	void SetFather(Transform *father);

	/**
	 * @brief 添加子节点.  会同时更新新旧父节点的子节点列表
	 */
	void AddChild(Transform *child);

	/**
	 * @brief 移除子节点. 会同时更新新旧父节点的子节点列表
	 */
	void RemoveChild(Transform *child);

	/**
	 * @brief 获取父节点
	 */
	Transform *GetFather() const;

	/**
	 * @brief 根据索引获取子节点
	 * @param index 索引
	 */
	Transform *GetChild(int index) const;

	/**
	 * @brief 根据gid获取子节点的索引
	 * @param gid 子节点的gid
	 */
	int GetIndex(const int gid) const;

	/**
	 * @brief 遍历所有子节点
	 * @param func 遍历函数
	 */
	void ForEachChild(std::function<void(Transform *)> func) const;

	/**
	 * @brief 根据相对路径查找Transform
	 * @param relativePath 相对路径, 如: "root/child1/child2"
	 */
	Transform *FindByPath(const string &relativePath) const;

	/**
	 * @brief 根据名字查找子节点
	 * @param name 名字
	 */
	Transform *FindByName(const string &name) const;

	/**
	 * @brief 根据gid查找子节点
	 * @param gid gid
	 */
	Transform *FindByGid(const int gid) const;

	/**
	 * @brief 获取子节点数量
	 */
	int GetChildCount() const;

	// 用于序列化的父节点gid
	int fatherGID_forSerialize;
	// 用于序列化的子节点gid
	vector<int> childrenGIDs_forSerialize;

private:
	Transform *_father;
	vector<Transform *> *_children;
	string GetName() const;
};
