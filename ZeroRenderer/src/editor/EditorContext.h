#pragma once
#include "MaterialRepo.h"
#include "ShaderRepo.h"
#include "TextureRepo.h"
#include "Camera3D.h"
#include "DirectLight.h"
#include "Scene.h"

class EditorContext {

public:
	EditorContext();
	~EditorContext();

	MaterialRepo* GetMaterialRepo();
	ShaderRepo* GetShaderRepo();
	TextureRepo* GetTextureRepo();

	Camera3D* sceneViewCamera;
	DirectLight* sceneDirectLight;
	Scene* currentScene;

private:
	MaterialRepo* _materialRepo;
	ShaderRepo* _shaderRepo;
	TextureRepo* _textureRepo;

};
