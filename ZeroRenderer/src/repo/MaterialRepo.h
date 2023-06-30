#pragma once
#include <unordered_map>
#include <Material.h>

class MaterialRepo {

public:
	MaterialRepo();
	~MaterialRepo();

	bool TryAddMaterial(const string& guid, Material*& material);
	bool TryGetMaterialByGUID(const std::string& guid, Material*& material);

private:
	std::unordered_map<std::string, Material*> allMaterials_sortedByPath;
	std::unordered_map<std::string, Material*> allMaterials_sortedByGUID;

};

