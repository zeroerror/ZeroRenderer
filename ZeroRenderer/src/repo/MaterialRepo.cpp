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

bool MaterialRepo::TryLoadMaterial(const char* path, Material*& material) {
	string guid;
	if (!Database::TryGetGUIDFromAssetPath(path, guid)) {
		return false;
	}

	if (_TryGetMaterialByGUID(guid, material)) {
		return true;
	}

	if (!Database::TryLoadMaterialByGUID(guid, material)) {
		return false;
	}

	allMaterials.insert(std::pair<std::string, Material*>(guid, material));
	return true;
}

bool MaterialRepo::_TryGetMaterialByGUID(const std::string& guid, Material*& material) {
	std::unordered_map<std::string, Material*>::iterator it = allMaterials.find(guid);
	if (it == allMaterials.end()) {
		std::cout << "  ################ _TryGetMaterialByGUID: " << guid << " not found" << std::endl;
		return false;
	}

	material = it->second;
	return true;
}