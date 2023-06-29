#pragma once
#include <unordered_map>
#include <Material.h>

class MaterialRepo {

public:
	MaterialRepo();
	~MaterialRepo();

	bool TryLoadMaterial(const char* path, Material*& material);

private:
	std::unordered_map<std::string, Material*> allMaterials;
	bool _TryGetMaterialByGUID(const std::string& guid, Material*& material);

};

