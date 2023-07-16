#pragma once
#include "MaterialRepo.h"
#include "ShaderRepo.h"
#include "TextureRepo.h"
#include "MeshRepo.h"
#include "SceneRepo.h"

#include "Camera.h"
#include "DirectLight.h"

class RuntimeContext {

public:
	RuntimeContext();
	~RuntimeContext();

	MaterialRepo* GetMaterialRepo();
	ShaderRepo* GetShaderRepo();
	TextureRepo* GetTextureRepo();
	MeshRepo* GetMeshRepo();
	SceneRepo* GetSceneRepo();

	Camera* sceneViewCamera;
	DirectLight* sceneDirectLight;
	Scene* currentScene;

private:
	MaterialRepo* _materialRepo;
	ShaderRepo* _shaderRepo;
	TextureRepo* _textureRepo;
	MeshRepo* _meshRepo;
	SceneRepo* _sceneRepo;

};

