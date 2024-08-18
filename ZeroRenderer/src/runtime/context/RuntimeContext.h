#pragma once
#include "MaterialRepo.h"
#include "ShaderRepo.h"
#include "TextureRepo.h"
#include "MeshRepo.h"
#include "SceneRepo.h"

#include "Camera.h"
#include "DirectLight.h"

class RuntimeContext
{

public:
	RuntimeContext();
	~RuntimeContext();

	MaterialRepo *GetMaterialRepo();
	ShaderRepo *GetShaderRepo();
	TextureRepo *GetTextureRepo();
	MeshRepo *GetMeshRepo();
	SceneRepo *GetSceneRepo();

	/** 主相机 */
	Camera *mainCamera;
	/** 主光源 */
	DirectLight *sceneDirectLight;
	/** 当前场景 */
	Scene *currentScene;

	/** 阴影贴图的FBO */
	unsigned int currentSceneShadowMapFBO;
	/** 阴影贴图的纹理 */
	unsigned int currentSceneShadowMapTexture;
	/** 阴影贴图的大小 */
	unsigned int currentSceneShadowMapSize;

private:
	MaterialRepo *_materialRepo;
	ShaderRepo *_shaderRepo;
	TextureRepo *_textureRepo;
	MeshRepo *_meshRepo;
	SceneRepo *_sceneRepo;
};
