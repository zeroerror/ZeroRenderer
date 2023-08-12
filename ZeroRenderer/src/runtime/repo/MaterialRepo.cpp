#include "MaterialRepo.h"
#include <iostream>

#include "EditorDatabase.h"

MaterialRepo::MaterialRepo() {
	allMaterials_sortedByPath = std::unordered_map<std::string, Material*>();
	allMaterials_sortedByGUID = std::unordered_map<std::string, Material*>();
}

MaterialRepo::~MaterialRepo() {
	std::cout << "MaterialRepo::~MaterialRepo() " << std::endl;
	for (auto& Material : allMaterials_sortedByPath) {
		Material.second->~Material();
	}
}

bool MaterialRepo::TryAddMaterial(const std::string& guid, Material*& material) {
	if(!EditorDatabase::GUIDExist(guid)) {
		return false;
	}

	allMaterials_sortedByGUID.insert(std::pair<std::string, Material*>(guid, material));
	
	string path;
	if (EditorDatabase::TryGetAssetPathFromGUID(guid, path)) {
		allMaterials_sortedByPath.insert(std::pair<std::string, Material*>(path, material));
	}

	std::cout << "MaterialRepo::AddMaterial: " << guid << std::endl;
	return true;
}

bool MaterialRepo::TryGetMaterialByGUID(const std::string& guid, Material*& material) {
	std::unordered_map<std::string, Material*>::iterator it = allMaterials_sortedByGUID.find(guid);
	if (it == allMaterials_sortedByGUID.end()) {
		return false;
	}

	material = it->second;
	return true;
}
