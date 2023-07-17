#include "RuntimeContext.h"

RuntimeContext::RuntimeContext() {
	_materialRepo = new MaterialRepo();
	_shaderRepo = new ShaderRepo();
	_textureRepo = new TextureRepo();
	_meshRepo = new MeshRepo();
	_sceneRepo = new SceneRepo();
}

RuntimeContext::~RuntimeContext() {
	delete _materialRepo;
	delete _shaderRepo;
	delete _textureRepo;
	delete mainCamera;
}

MaterialRepo* RuntimeContext::GetMaterialRepo() { return _materialRepo; }
ShaderRepo* RuntimeContext::GetShaderRepo() { return _shaderRepo; }
TextureRepo* RuntimeContext::GetTextureRepo() { return _textureRepo; }
MeshRepo* RuntimeContext::GetMeshRepo() { return _meshRepo; }
SceneRepo* RuntimeContext::GetSceneRepo() { return _sceneRepo; }