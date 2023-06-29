#pragma once
#include "MaterialRepo.h"
#include "ShaderRepo.h"
#include "TextureRepo.h"
#include "Camera3D.h"

class EditorContext {

public:
	EditorContext();
	~EditorContext();

	MaterialRepo* GetMaterialRepo();
	ShaderRepo* GetShaderRepo();
	TextureRepo* GetTextureRepo();

	Camera3D* _sceneViewCamera;

private:
	MaterialRepo* _materialRepo;
	ShaderRepo* _shaderRepo;
	TextureRepo* _textureRepo;

};

