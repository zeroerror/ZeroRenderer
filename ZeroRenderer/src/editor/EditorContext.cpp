#include "EditorContext.h"

EditorContext::EditorContext() {
	_materialRepo = new MaterialRepo();
	_shaderRepo = new ShaderRepo();
	_textureRepo = new TextureRepo();
	_meshRepo = new MeshRepo();
	_sceneRepo = new SceneRepo();
	sceneViewCamera = new Camera();
}

EditorContext::~EditorContext() {
	delete _materialRepo;
	delete _shaderRepo;
	delete _textureRepo;
	delete sceneViewCamera;
}

MaterialRepo* EditorContext::GetMaterialRepo() { return _materialRepo; }
ShaderRepo* EditorContext::GetShaderRepo() { return _shaderRepo; }
TextureRepo* EditorContext::GetTextureRepo() { return _textureRepo; }
MeshRepo* EditorContext::GetMeshRepo() { return _meshRepo; }
SceneRepo* EditorContext::GetSceneRepo() { return _sceneRepo; }



