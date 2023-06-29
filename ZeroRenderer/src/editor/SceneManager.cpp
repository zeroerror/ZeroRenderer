#include "SceneManager.h"
#include <src/material/Material.h>
#include "Scene.h"
#include "Database.h"

Scene* SceneManager::scene;

void SceneManager::LoadScene() {

	unsigned int scrWidth = 1920;
	unsigned int scrHeight = 1080;

	// ======================== Scene
	scene = new Scene();

	scene->camera->scrWidth = scrWidth;
	scene->camera->scrHeight = scrHeight;
	scene->camera->transform->SetPosition(glm::vec3(0, 10, -10));
	scene->camera->transform->SetRotation(glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f))));

	// Light 
	scene->directLight = new DirectLight();
	scene->directLight->scrWidth = scrWidth;
	scene->directLight->scrHeight = scrHeight;
	scene->directLight->shadowType = ShadowType::Hard;
	scene->directLight->transform->SetPosition(scene->camera->transform->GetPosition());
	scene->directLight->transform->SetRotation(scene->camera->transform->GetRotation());
	scene->directLight->fov = scene->camera->fov;
	scene->directLight->scrWidth = scene->camera->scrWidth;
	scene->directLight->scrHeight = scene->camera->scrHeight;
	scene->directLight->nearPlane = scene->camera->nearPlane;
	scene->directLight->farPlane = scene->camera->farPlane;
	scene->directLight->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	Material* defaultMaterial = new Material();
	Database::TryLoadMaterialByAssetPath("asset/material/default.mat", defaultMaterial);
	Material* defaultLightMaterial = new Material();
	Database::TryLoadMaterialByAssetPath("asset/material/defaultLight.mat", defaultLightMaterial);
	Material* lightCubeMaterial = new Material();
	Database::TryLoadMaterialByAssetPath("asset/material/lightCube.mat", lightCubeMaterial);
	Material* depthMapMaterial = new Material();
	Database::TryLoadMaterialByAssetPath("asset/material/depthMap.mat", depthMapMaterial);

	// Create a central light source cube
	scene->lightCube = Cube::CreateCube(0.2f, 0.2f, 1.0f);
	scene->lightCube->material = lightCubeMaterial;

	// Create a depth map 2D image
	scene->depthMapImage = Rectangle::CreateRectangle(16.0f, 9.0f);
	scene->depthMapImage->transform->SetPosition(glm::vec3(0.0f, 10.0f, 10.0f));
	scene->depthMapImage->transform->SetRotation(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
	scene->depthMapImage->material = depthMapMaterial;

	// Create the ground
	Cube* groundCube = Cube::CreateCube(20.0f, 0.1f, 30.0f);
	groundCube->transform->SetPosition(glm::vec3(0.0f, -0.05f, 0.0f));
	groundCube->material = defaultLightMaterial;
	scene->cubes->push_back(groundCube);

	// Create walls
	Cube* wall1 = Cube::CreateCube(1.0f, 5.0f, 10.0f);
	wall1->transform->SetPosition(glm::vec3(-8.0f, 2.5f, 0.0f));
	wall1->material = defaultLightMaterial;
	scene->cubes->push_back(wall1);

	Cube* wall2 = Cube::CreateCube(10.0f, 5.0f, 1.0f);
	wall2->transform->SetPosition(glm::vec3(0.0f, 2.5f, -8.0f));
	wall2->material = defaultLightMaterial;
	scene->cubes->push_back(wall2);

	// Create obstacles
	Cube* obstacle1 = Cube::CreateCube(2.0f, 2.0f, 2.0f);
	obstacle1->transform->SetPosition(glm::vec3(-4.0f, 1.0f, 4.0f));
	obstacle1->material = defaultLightMaterial;
	scene->cubes->push_back(obstacle1);

	Cube* obstacle2 = Cube::CreateCube(2.0f, 2.0f, 2.0f);
	obstacle2->transform->SetPosition(glm::vec3(4.0f, 1.0f, -4.0f));
	obstacle2->material = defaultLightMaterial;
	scene->cubes->push_back(obstacle2);

	Cube* obstacle3 = Cube::CreateCube(3.0f, 1.0f, 2.0f);
	obstacle3->transform->SetPosition(glm::vec3(6.0f, 0.5f, 6.0f));
	obstacle3->material = defaultLightMaterial;
	scene->cubes->push_back(obstacle3);

	// ========================== Load Model
	Model* model = new Model();
	model->LoadModel("asset/model/nanosuit/nanosuit.obj");
	model->transform->SetPosition(glm::vec3(0.0f, 0.0f, 20.0f));
	model->transform->SetRotation(glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(180.0f), glm::radians(0.0f))));
	scene->models->push_back(model);
}