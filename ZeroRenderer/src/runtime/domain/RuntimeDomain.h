#pragma once
#include "SceneMeta.h"
#include "ShaderMeta.h"
#include "ObjMeta.h"
#include "PrefabInstanceMeta.h"
#include "PrefabMeta.h"
#include "GameObjectMeta.h"
#include "ComponentMeta.h"
#include "CameraMeta.h"
#include "DirectLightMeta.h"
#include "MeshFilterMeta.h"
#include "MeshRendererMeta.h"
#include "SkinMeshRendererMeta.h"

#include "TextureSlotCollection.h"
#include "SkinMeshRenderer.h"
#include "RuntimeContext.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class RuntimeDomain
{

	RuntimeContext *_runtimeContext;

public:
	/**
	 * 注入运行时上下文
	 * @param ctxt 运行时上下文指针
	 */
	void Inject(RuntimeContext *ctxt);

	/**
	 * 预处理模型网格
	 */
	void PreprocessModelMeshes();

	/**
	 * 从指定路径预处理模型网格
	 * @param path 模型网格的路径
	 */
	void PreprocessModelMeshFromPath(const string &path);

	/**
	 * 将 SMR 元数据预处理为网格
	 * @param aScene Assimp 场景指针
	 * @param skinMeshRendererMeta 皮肤网格渲染器元数据
	 */
	void PreprocessSMRMetaToMesh(const aiScene *aScene, const SkinMeshRendererMeta &skinMeshRendererMeta);

	/**
	 * 将模型处理为网格
	 * @param aScene Assimp 场景指针
	 * @param meshIndex 网格索引
	 * @param mesh 网格指针
	 */
	void ProcessModelToMesh(const aiScene *aScene, const int &meshIndex, Mesh *mesh);

	/**
	 * 绑定着色器
	 * @param transform 变换指针
	 * @param shader 着色器指针
	 * @param camera 相机
	 */
	void BindShader(const Transform *transform, Shader *shader, const Camera &camera);

	/**
	 * 尝试通过 GUID 加载材质
	 * @param guid 材质的 GUID
	 * @param mat 材质指针的引用
	 * @return 是否成功加载
	 */
	bool TryLoadMaterialByGUID(const string &guid, Material *&mat);

	/**
	 * 尝试通过资产路径加载材质
	 * @param path 材质的资产路径
	 * @param mat 材质指针的引用
	 * @return 是否成功加载
	 */
	bool TryLoadMaterialByAssetPath(const string &path, Material *&mat);

	/**
	 * 对 SkinMeshRenderer 进行批处理
	 * @param skinMeshRenderer 指向 SkinMeshRenderer 的指针
	 */
	void BatchSkinMeshRenderer(SkinMeshRenderer *skinMeshRenderer);

	/**
	 * 批量绘制 SkinMeshRenderer
	 * @param skinMeshRenderer 指向 SkinMeshRenderer 的指针
	 */
	void BatchedDrawSkinMeshRenderer(SkinMeshRenderer *skinMeshRenderer);

	/**
	 * 打开场景
	 * @param path 场景文件的路径
	 * @param resSceneMeta 用于存储场景元数据的引用
	 * @return 指向打开的场景的指针
	 */
	Scene *OpenScene(const string &path, SceneMeta &resSceneMeta);

	/**
	 * 渲染场景
	 * @param scene 要渲染的场景
	 * @param camera 相机
	 */
	void RenderScene(const Scene &scene, const Camera &camera);

	/**
	 * 渲染场景的阴影图
	 * @param scene 要渲染的场景
	 * @param camera 相机
	 */
	void RendererSceneShadowMap(const Scene &scene, const Camera &camera);

	/**
	 * 绘制 SkinMeshRenderer
	 * @param skinMeshRenderer SkinMeshRenderer
	 * @param camera 相机
	 */
	void DrawSkinMeshRenderer(const SkinMeshRenderer *skinMeshRenderer, const Camera &camera);

	/**
	 * 绘制 MeshRenderer
	 * @param meshRenderer MeshRenderer
	 * @param camera 相机
	 */
	void DrawMeshRenderer(const MeshRenderer *meshRenderer, const Camera &camera);

	// ============================ All meta's methods that convert to a runtime game object. =====================================
	void MetaToDirectLight(const DirectLightMeta &directLightMeta, DirectLight &directLight);
	void MetaToCamera(const CameraMeta &cameraMeta, Camera &camera);
	void MetaToMeshFilter(const MeshFilterMeta &meshFilterMeta, MeshFilter &meshFilter);
	void MetaToMeshRenderer(const MeshRendererMeta &meshRendererMeta, MeshRenderer &meshRenderer);
	void MetaToSkinMeshRenderer(const SkinMeshRendererMeta &skinMeshRendererMeta, SkinMeshRenderer &skinMeshRenderer);
	void MetaToTransform(const TransformMeta &transformMeta, Transform &transform);
	void GUIDToPrefabMeta(const string &guid, PrefabMeta &prefabMeta);
	void MetaToGameObject(const PrefabMeta &prefabMeta, GameObject &gameObject);
	void MetaToGameObject(const PrefabInstanceMeta &prefabInstanceMeta, GameObject &gameObject);
	void MetaToGameObject(const GameObjectMeta &gameObjectMeta, GameObject &gameObject);
	void MetaToScene(const SceneMeta &sceneMeta, Scene &scene);

	// ============================ All asset's methods that convert to a runtime game object. =====================================
	GameObject* assetToGameObject(const string guid);

private:
	void _MetaToGameObject(const vector<ComponentMeta *> componentMetas, GameObject &gameObject);
};
