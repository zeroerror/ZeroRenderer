#include "EditorContext.h"

EditorContext::EditorContext() {
	_materialRepo = new MaterialRepo();
	_shaderRepo = new ShaderRepo();
	_textureRepo = new TextureRepo();
	_sceneViewCamera = new Camera3D();
}

EditorContext::~EditorContext() {
	delete _materialRepo;
	delete _shaderRepo;
	delete _textureRepo;
	delete _sceneViewCamera;
}

MaterialRepo* EditorContext::GetMaterialRepo() { return _materialRepo; }
ShaderRepo* EditorContext::GetShaderRepo() { return _shaderRepo; }
TextureRepo* EditorContext::GetTextureRepo() { return _textureRepo; }


