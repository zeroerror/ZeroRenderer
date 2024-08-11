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

	/**
	 * @brief 获取世界坐标系下的坐标
	 */
	vec3 GetPosition() const;

	/**
	 * @brief 设置世界坐标系下的坐标
	 * @param newPosition 新的世界坐标系下的坐标
	 */
	void SetPosition(const vec3 &newPosition);

	/**
	 * @brief 获取世界坐标系下的旋转
	 */
	quat GetRotation() const;

	/**
	 * @brief 设置世界坐标系下的旋转
	 * @param newRotation 新的世界坐标系下的旋转
	 */
	void SetRotation(const quat &newRotation);

	/**
	 * @brief 设置世界坐标系下的旋转
	 * @param eulerAngles 新的世界坐标系下的旋转
	 */
	void SetRotation(const vec3 &eulerAngles);

	/**
	 * @brief 获取世界坐标系下的缩放
	 */
	vec3 GetScale() const;

	/**
	 * @brief 设置世界坐标系下的缩放
	 * @param newScale 新的世界坐标系下的缩放
	 */
	void SetScale(const vec3 &newScale);

	/**
	 * @brief 获取本地坐标系下的坐标
	 */
	vec3 GetLocalPosition() const;

	/**
	 * @brief 设置本地坐标系下的坐标
	 * @param newPosition 新的本地坐标系下的坐标
	 */
	void SetLocalPosition(const vec3 &newPosition);

	/**
	 * @brief 获取本地坐标系下的旋转
	 */
	quat GetLocalRotation() const;

	/**
	 * @brief 设置本地坐标系下的旋转
	 * @param newRotation 新的本地坐标系下的旋转
	 */
	void SetLocalRotation(const quat &newRotation);

	/**
	 * @brief 设置本地坐标系下的旋转
	 * @param eulerAngles 新的本地坐标系下的旋转
	 */
	void SetLocalRotation(const vec3 &eulerAngles);

	/**
	 * @brief 获取本地坐标系下的缩放
	 */
	vec3 GetLocalScale() const;

	/**
	 * @brief 设置本地坐标系下的缩放
	 * @param newScale 新的本地坐标系下的缩放
	 */
	void SetLocalScale(const vec3 &newScale);

	/**
	 * @brief 获取世界坐标系下的前方向
	 */
	vec3 GetForward() const;

	/**
	 * @brief 获取世界坐标系下的上方向
	 */
	vec3 GetUp() const;

	/**
	 * @brief 获取世界坐标系下的右方向
	 */
	vec3 GetRight() const;

	/**
	 * @brief 深度优先遍历所有子节点
	 */
	void DFSChildren(std::function<void(Transform *)> callback);

private:
	vec3 _position;
	quat _rotation;
	vec3 _scale;

	vec3 _localPosition;
	quat _localRotation;
	vec3 _localScale;

	vec3 m_forward;
	vec3 m_up;
	vec3 m_right;

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
	 * @brief 更新本地坐标系下的坐标
	 */
	void UpdateLocalTRS_T();

	/**
	 * @brief 更新本地坐标系下的坐标
	 */
	void UpdateLocalTRS_R();

	/**
	 * @brief 更新本地坐标系下的缩放
	 */
	void UpdateLocalTRS_S();

	/**
	 * @brief 更新本地坐标系下的TRS
	 */
	void UpdateLocalTRS();

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
