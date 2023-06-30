#include "MaterialRepo.h"
#include <iostream>

#include "Database.h"

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
	if(!Database::GUIDExist(guid)) {
		return false;
	}

	allMaterials_sortedByGUID.insert(std::pair<std::string, Material*>(guid, material));
	
	string path;
	if (Database::TryGetAssetPathFromGUID(guid, path)) {
		allMaterials_sortedByPath.insert(std::pair<std::string, Material*>(path, material));
	}

	std::cout << "MaterialRepo::TryAddMaterial: " << guid << std::endl;
	return true;
}

bool MaterialRepo::TryGetMaterialByGUID(const std::string& guid, Material*& material) {
	std::unordered_map<std::string, Material*>::iterator it = allMaterials_sortedByGUID.find(guid);
	if (it == allMaterials_sortedByGUID.end()) {
		std::cout << "  ################ _TryGetMaterialByGUID: " << guid << " not found" << std::endl;
		return false;
	}

	material = it->second;
	return true;
}