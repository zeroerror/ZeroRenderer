#include "MaterialRepo.h"
#include <iostream>

#include "Database.h"

MaterialRepo::MaterialRepo() {
	allMaterials = std::unordered_map<std::string, Material*>();
}

MaterialRepo::~MaterialRepo() {
	std::cout << "MaterialRepo::~MaterialRepo() " << std::endl;
	for (auto& Material : allMaterials) {
		Material.second->~Material();
	}
}

std::string MaterialRepo::LoadMaterial(const char* path) {
	Material* material = new Material();
	std::string guid = Database::GetGUIDFromAssetPath(path);
	allMaterials.insert(std::pair<std::string, Material*>(guid, material));
	std::cout << "MaterialRepo::LoadMaterial **** path - " << path << " GUID - " << guid << std::endl;
	return guid;
}

Material* MaterialRepo::GetMaterialByGUID(const std::string& guid) {
	std::unordered_map<std::string, Material*>::iterator it = allMaterials.find(guid);
	if (it != allMaterials.end()) {
		return allMaterials.at(guid);
	}

	std::cout << "################ GetMaterialByGUID: " << guid << " not found" << std::endl;
	return nullptr;
}