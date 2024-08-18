#include "RuntimeContext.h"

RuntimeContext::RuntimeContext()
{
	_shaderRepo = new ShaderRepo();
	_textureRepo = new TextureRepo();
	_materialRepo = new MaterialRepo();
	_meshRepo = new MeshRepo();
	_sceneRepo = new SceneRepo();

	Material *defalutMaterial = new Material();
	defalutMaterial->shader = _shaderRepo->DefaultLightShader();
	defalutMaterial->diffuseTexture = _textureRepo->DefaultDiffuseTexture();
	_materialRepo->defaultMaterial = defalutMaterial;

	Material *errorMat = new Material();
	errorMat->shader = _shaderRepo->ErrorShader();
	errorMat->diffuseTexture = _textureRepo->DefaultDiffuseTexture();
	_materialRepo->errorMat = errorMat;

	currentSceneShadowMapSize = 4096;
}

RuntimeContext::~RuntimeContext()
{
	delete _materialRepo;
	delete _shaderRepo;
	delete _textureRepo;
	delete mainCamera;
}

MaterialRepo *RuntimeContext::GetMaterialRepo() { return _materialRepo; }
ShaderRepo *RuntimeContext::GetShaderRepo() { return _shaderRepo; }
TextureRepo *RuntimeContext::GetTextureRepo() { return _textureRepo; }
MeshRepo *RuntimeContext::GetMeshRepo() { return _meshRepo; }
SceneRepo *RuntimeContext::GetSceneRepo() { return _sceneRepo; }